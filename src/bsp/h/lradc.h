#ifndef __LRADC_H
#define __LRADC_H

#define  LRADC_SAMPLE_250H           (0x00)
#define  LRADC_SAMPLE_125H           (0x01)
#define  LRADC_SAMPLE_63H            (0x02)
#define  LRADC_SAMPLE_32H            (0x03)


#define  LRADC_LEVELB_1_6V           (0x00)
#define  LRADC_LEVELB_1_7V           (0x01)
#define  LRADC_LEVELB_1_8V           (0x02)
#define  LRADC_LEVELB_1_9V           (0x03)



#define  LRADC_CHANNEL_ONE           (0x00)
#define  LRADC_CHANNEL_TWO           (0x01)

#define  LRADC_SAMPLE_DEFAULT        LRADC_SAMPLE_125H
#define  LRADC_LEVELB_DEFAULT        LRADC_LEVELB_1_8V
#define  LRADC_CHANNEL_SELECT        LRADC_CHANNEL_ONE

#endif
