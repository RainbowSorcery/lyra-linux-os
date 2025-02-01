
void irq_init();

void gate_desc_set(gate_desc_t *gdt, uint16_t selector, unint32_t offset, uint16_t attr);