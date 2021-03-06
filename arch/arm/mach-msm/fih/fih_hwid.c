#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/io.h>

#include <fih/hwid.h>

#define FIH_HWID_ADDR  0x10880000
#define FIH_HWID_SIZE  64

/* conserve current hwid table in kernel use */
struct st_hwid_table fih_hwid_table;

/* default coded hwid table for development use */
struct st_hwid_table def_hwid_table = {
	/* mpp */
	.r1 = 0,
	.r2 = 0,
	.r3 = 0,
	/* info */
	.prj = FIH_PRJ_PHX,
	.rev = FIH_REV_MP,
	.rf  = FIH_RF_G_850_900_1800_1900_W_1_2_L_1_3_5_7_8_9,
	/* device tree */
	.dtm = 4,
	.dtn = 2,
	/* driver */
	.btn = FIH_BTN_PMIC_UP_5_DN_2,
	.uart = FIH_UART_TX_4_RX_5,
};

void fih_hwid_setup(void)
{
	struct st_hwid_table *mem_hwid_table = (struct st_hwid_table *)ioremap(FIH_HWID_ADDR, sizeof(struct st_hwid_table));
	if (mem_hwid_table == NULL) {
		pr_err("%s: setup hwid table by default coded because load fail\n", __func__);
		memcpy(&fih_hwid_table, &def_hwid_table, sizeof(struct st_hwid_table));
		return;
	}

	memcpy(&fih_hwid_table, mem_hwid_table, sizeof(struct st_hwid_table));
}

void fih_hwid_read(struct st_hwid_table *tb)
{
	memcpy(tb, &fih_hwid_table, sizeof(struct st_hwid_table));
}

int fih_hwid_fetch(int idx)
{
	struct st_hwid_table *tb = &fih_hwid_table;
	int ret = (-1);

	switch (idx) {
		/* mpp */
		case FIH_HWID_R1: ret = tb->r1; break;
		case FIH_HWID_R2: ret = tb->r2; break;
		case FIH_HWID_R3: ret = tb->r3; break;
		/* info */
		case FIH_HWID_PRJ: ret = tb->prj; break;
		case FIH_HWID_REV: ret = tb->rev; break;
		case FIH_HWID_RF: ret = tb->rf; break;
		/* device tree */
		case FIH_HWID_DTM: ret = tb->dtm; break;
		case FIH_HWID_DTN: ret = tb->dtn; break;
		/* driver */
		case FIH_HWID_BTN: ret = tb->btn; break;
		case FIH_HWID_UART: ret = tb->uart; break;
		default:
			pr_warn("fih_hwid_fetch: unknown idx = %d\n", idx);
			ret = (-1);
			break;
	}

	return ret;
}
