#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/init_task.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>


//asmlinkage unsigned long sys_get_address( unsigned long vaddr )
SYSCALL_DEFINE1( get_address, unsigned long , vaddr )
{

    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned long paddr = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;

    printk( "%p:\n", vaddr );
    pgd = pgd_offset(current->mm, vaddr);
    printk("pgd_val = 0x%lx\n", pgd_val(*pgd));
    printk("pgd_index = %lu\n", pgd_index(vaddr));
    if (pgd_none(*pgd)) {
        printk("not mapped in pgd\n");
        return -1;
    }
    
    p4d = p4d_offset(pgd, vaddr);
    printk("p4d_val = 0x%lx\n", p4d_val(*p4d));
    if (p4d_none(*p4d)) {
        printk("not mapped in pgd\n");
        return -1;
    }

    pud = pud_offset(p4d, vaddr);
    printk("pud_val = 0x%lx\n", pud_val(*pud));
    if (pud_none(*pud)) {
        printk("not mapped in pud\n");
        return -1;
    }

    pmd = pmd_offset(pud, vaddr);
    printk("pmd_val = 0x%lx\n", pmd_val(*pmd));
    printk("pmd_index = %lu\n", pmd_index(vaddr));
    if (pmd_none(*pmd)) {
        printk("not mapped in pmd\n");
        return -1;
    }

    pte = pte_offset_kernel(pmd, vaddr);
    printk("pte_val = 0x%lx\n", pte_val(*pte));
    printk("pte_index = %lu\n", pte_index(vaddr));
    if (pte_none(*pte)) {
        printk("not mapped in pte\n");
        return -1;
    }

    /* Page frame physical address mechanism | offset */
    page_addr = pte_val(*pte) & PAGE_MASK;
    page_offset = vaddr & ~PAGE_MASK;
    paddr = page_addr | page_offset;
    printk("page_addr = %lx, page_offset = %lx\n", page_addr, page_offset);
    printk("vaddr = %lx, paddr = %lx\n", vaddr, paddr);

    printk( "end\n\n" );
    return paddr;


}
