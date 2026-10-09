# Zilog Z8000 DMA

DMA is modeled at system level through SLDMAController and SLBusArbiter. A DMA-capable peripheral requests the bus, transfers data to or from memory, and releases the bus. No CPU-integrated DMA engine is assumed without implementation evidence.
