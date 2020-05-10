#include <linux/kernel.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/module.h>
//#include <asm-generic/io.h>
#include <linux/io.h>

#define PAD_LEDCON 0x01c208b4
#define PAD_LEDDAT 0x01c208c4
unsigned int *pad_con = NULL;
unsigned int *pad_dat = NULL;


struct led_classdev led_dev;

void ledd_brightness_set(struct led_classdev * led_dev,enum led_brightness brightness)
{
	if(brightness){
		writel((readl(pad_dat)&(~(1<<2))),pad_dat);
	}else {
		writel((readl(pad_dat)|(1<<2)),pad_dat);
	}

}

static int __init led_init(void)
{
	int ret;
	led_dev.name = "myled";
	led_dev.brightness_set = ledd_brightness_set;
	led_dev.flags |= LED_CORE_SUSPENDRESUME;
	led_dev.max_brightness= LED_FULL;


	pad_con = (unsigned int *)ioremap(PAD_LEDCON,4);
	pad_dat = (unsigned int *)ioremap(PAD_LEDDAT,4);
	writel(((readl(pad_con)&(~(0xf<<8)))|(1<<8)),pad_con);
	writel((readl(pad_dat)|(1<<2)),pad_dat);

	ret = led_classdev_register(NULL,&led_dev);
	if(ret < 0){
		printk("led register is fail \n");
		return -EAGAIN;
	}

	return 0;

}

static void __exit led_exit(void)
{
	led_classdev_unregister(&led_dev);
}
module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");

