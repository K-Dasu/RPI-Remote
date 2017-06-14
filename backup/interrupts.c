
#include <stdint.h>
#include <stdbool.h>

#define ARM_IC_IRQ_REGS		3

#define ARM_IC_IRQ_PENDING(irq)	(  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_IRQ_PENDING_1		\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_IRQ_PENDING_2	\
				   : ARM_IC_IRQ_BASIC_PENDING))
#define ARM_IC_IRQS_ENABLE(irq)	(  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_ENABLE_IRQS_1		\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_ENABLE_IRQS_2	\
				   : ARM_IC_ENABLE_BASIC_IRQS))
#define ARM_IC_IRQS_DISABLE(irq) (  (irq) < ARM_IRQ2_BASE	\
				 ? ARM_IC_DISABLE_IRQS_1	\
				 : ((irq) < ARM_IRQBASIC_BASE	\
				   ? ARM_IC_DISABLE_IRQS_2	\
				   : ARM_IC_DISABLE_BASIC_IRQS))
#define ARM_IRQ_MASK(irq)	(1 << ((irq) & (ARM_IRQS_PER_REG-1)))


boolean CInterruptSystem::Initialize (void)
{
	TExceptionTable *pTable = (TExceptionTable *) ARM_EXCEPTION_TABLE_BASE;
	pTable->IRQ = ARM_OPCODE_BRANCH (ARM_DISTANCE (pTable->IRQ, IRQStub));
#ifndef USE_RPI_STUB_AT
	pTable->FIQ = ARM_OPCODE_BRANCH (ARM_DISTANCE (pTable->FIQ, FIQStub));
#endif

	SyncDataAndInstructionCache ();

#ifndef USE_RPI_STUB_AT
	PeripheralEntry ();

	write32 (ARM_IC_FIQ_CONTROL, 0);

	write32 (ARM_IC_DISABLE_IRQS_1, (u32) -1);
	write32 (ARM_IC_DISABLE_IRQS_2, (u32) -1);
	write32 (ARM_IC_DISABLE_BASIC_IRQS, (u32) -1);

	PeripheralExit ();
#endif

	EnableIRQs ();

	return TRUE;
}

void CInterruptSystem::ConnectIRQ (unsigned nIRQ, TIRQHandler *pHandler, void *pParam)
{
	assert (nIRQ < IRQ_LINES);
	assert (m_apIRQHandler[nIRQ] == 0);

	m_apIRQHandler[nIRQ] = pHandler;
	m_pParam[nIRQ] = pParam;

	EnableIRQ (nIRQ);
}

void CInterruptSystem::DisconnectIRQ (unsigned nIRQ)
{
	assert (nIRQ < IRQ_LINES);
	assert (m_apIRQHandler[nIRQ] != 0);

	DisableIRQ (nIRQ);

	m_apIRQHandler[nIRQ] = 0;
	m_pParam[nIRQ] = 0;
}

void CInterruptSystem::ConnectFIQ (unsigned nFIQ, TFIQHandler *pHandler, void *pParam)
{
#ifdef USE_RPI_STUB_AT
	assert (0);
#endif
	assert (nFIQ <= ARM_MAX_FIQ);
	assert (pHandler != 0);
	assert (FIQData.pHandler == 0);

	FIQData.pHandler = pHandler;
	FIQData.pParam = pParam;

	EnableFIQ (nFIQ);
}

void CInterruptSystem::DisconnectFIQ (void)
{
	assert (FIQData.pHandler != 0);

	DisableFIQ ();

	FIQData.pHandler = 0;
	FIQData.pParam = 0;
}

void CInterruptSystem::EnableIRQ (unsigned nIRQ)
{
	PeripheralEntry ();

	assert (nIRQ < IRQ_LINES);

	write32 (ARM_IC_IRQS_ENABLE (nIRQ), ARM_IRQ_MASK (nIRQ));

	PeripheralExit ();
}

void CInterruptSystem::DisableIRQ (unsigned nIRQ)
{
	PeripheralEntry ();

	assert (nIRQ < IRQ_LINES);

	write32 (ARM_IC_IRQS_DISABLE (nIRQ), ARM_IRQ_MASK (nIRQ));

	PeripheralExit ();
}

void CInterruptSystem::EnableFIQ (unsigned nFIQ)
{
	PeripheralEntry ();

	assert (nFIQ <= ARM_MAX_FIQ);

	write32 (ARM_IC_FIQ_CONTROL, nFIQ | 0x80);

	PeripheralExit ();
}

void CInterruptSystem::DisableFIQ (void)
{
	PeripheralEntry ();

	write32 (ARM_IC_FIQ_CONTROL, 0);

	PeripheralExit ();
}

CInterruptSystem *CInterruptSystem::Get (void)
{
	assert (s_pThis != 0);
	return s_pThis;
}

boolean CInterruptSystem::CallIRQHandler (unsigned nIRQ)
{
	assert (nIRQ < IRQ_LINES);
	TIRQHandler *pHandler = m_apIRQHandler[nIRQ];

	if (pHandler != 0)
	{
		(*pHandler) (m_pParam[nIRQ]);

		return TRUE;
	}
	else
	{
		DisableIRQ (nIRQ);
	}

	return FALSE;
}

void CInterruptSystem::InterruptHandler (void)
{
	assert (s_pThis != 0);

#ifdef ARM_ALLOW_MULTI_CORE
	if (CMultiCoreSupport::LocalInterruptHandler ())
	{
		return;
	}
#endif

	PeripheralEntry ();

	u32 Pending[ARM_IC_IRQ_REGS];
	Pending[0] = read32 (ARM_IC_IRQ_PENDING_1);
	Pending[1] = read32 (ARM_IC_IRQ_PENDING_2);
	Pending[2] = read32 (ARM_IC_IRQ_BASIC_PENDING) & 0xFF;

	PeripheralExit ();

	for (unsigned nReg = 0; nReg < ARM_IC_IRQ_REGS; nReg++)
	{
		u32 nPending = Pending[nReg];
		if (nPending != 0)
		{
			unsigned nIRQ = nReg * ARM_IRQS_PER_REG;

			do
			{
				if (   (nPending & 1)
				    && s_pThis->CallIRQHandler (nIRQ))
				{
					return;
				}

				nPending >>= 1;
				nIRQ++;
			}
			while (nPending != 0);
		}
	}
}

void InterruptHandler (void)
{
	PeripheralExit ();	// exit from interrupted peripheral

	CInterruptSystem::InterruptHandler ();

	PeripheralEntry ();	// continuing with interrupted peripheral
}
