#ifndef _ENC28J60_DEFINES
#define _ENC28J60_DEFINES


//Bank 0
#define ERDPTL	0x00
#define ERDPTH	0x01
#define EWRPTL	0x02
#define EWRPTH	0x03
#define ETXSTL	0x04
#define ETXSTH	0x05
#define ETXNDL	0x06
#define ETXNDH	0x07
#define ERXSTL	0x08
#define ERXSTH	0x09
#define ERXNDL	0x0A
#define ERXNDH	0x0B
#define ERXRDPTL	0x0C
#define ERXRDPTH	0x0D
#define ERXWRPTL	0x0E
#define ERXWRPTH	0x0F
#define EDMASTL		0x10
#define EDMASTH		0x11
#define EDMANDL		0x12
#define EDMANDH		0x13
#define EDMADSTL	0x14
#define EDMADSTH	0x15
#define EDMACSL		0x16
#define EDMACSH		0x17
#define EIE			0x1B
#define EIR			0x1C
#define ESTAT		0x1D
#define ECON2		0x1E
#define ECON1		0x1F

//Bank 1
#define EHT0	0x00
#define EHT1	0x01
#define EHT2	0x02
#define EHT3	0x03
#define EHT4	0x04
#define EHT5	0x05
#define EHT6	0x06
#define EHT7	0x07
#define EPMM0	0x08
#define EPMM1	0x09
#define EPMM2	0x0A
#define EPMM3	0x0B
#define EPMM4	0x0C
#define EPMM5	0x0D
#define EPMM6	0x0E
#define EPMM7	0x0F
#define EPMCSL	0x10
#define EPMCSH	0x11
#define EPMOL	0x14
#define EPMOH	0x15
#define ERXFCON	0x18
#define EPKTCNT	0x19


//Bank 2
#define MACON1	0x00
#define MACON3	0x02
#define MACON4	0x03
#define MABBIPG	0x04
#define MAIPGL		0x06
#define MAIPGH		0x07
#define MACLCON1	0x08
#define MACLCON2	0x09
#define MAMXFLL		0x0A
#define MAMXFLH		0x0B
#define MICMD		0x12
#define MIREGADR	0x14
#define MIWRL		0x16
#define MIWRH		0x17
#define MIRDL		0x18
#define MIRDH		0x19


//Bank 3
#define MAADR5	0x00
#define MAADR6	0x01
#define MAADR3	0x02
#define MAADR4	0x03
#define MAADR1	0x04
#define MAADR2	0x05
#define EBSTSD	0x06
#define EBSTCON	0x07
#define EBSTCSL	0x08
#define EBSTCSH	0x09
#define MISTAT	0x0A
#define EREVID		0x12
#define ECOCON		0x15
#define EFLOCON		0x17
#define EPAUSL		0x18
#define EPAUSH		0x19



#define READ_CONTROL_REG	0b00000000
#define READ_BUFFER_MEM		0b00100000
#define WRITE_CONTROL_REG	0b01000000 //WBM
#define WRITE_BUFFER_MEM	0b01100000 //WBM
#define	BIT_FIELD_SET		0b10000000
#define BIT_FIELD_CLEAR		0b10100000
#define SYSTEM_RESET		0b11100000

/*PHY REGISTERS... */
#define PHCON1	0x00
#define PHSTAT1	0x01
#define PHID1	0x02
#define PHID2	0x03
#define PHCON2	0x10
#define PHSTAT2	0x11
#define PHIE	0x12
#define PHIR	0x13
#define PHLCON	0x14


#define MAXIMUM_FRAME_SIZE	1518


#define RAMSIZE (0x1FFF) //8191
#define MAX_TX_PACKET_SIZE  (1518)
#define MIN_TX_PACKET_SIZE  (64)
#define MAX_TX_PACKETS (20)

#define TX_STATUS_VECTOR_SIZE   (7)

#define MIN_TX_PACKET           (MIN_TX_PACKET_SIZE + TX_STATUS_VECTOR_SIZE)
#define TX_BUFFER_SIZE          ((MAX_TX_PACKET_SIZE + TX_STATUS_VECTOR_SIZE) << 1) //3050


// typical memory map for the MAC buffers
#define TXSTART (RAMSIZE - TX_BUFFER_SIZE - 1) //TX start at 5141
#define TXEND	(RAMSIZE) // TX ends at 8191

#define RXSTART (0) //Start at address 0x00
#define RXEND	(TXSTART - 2) //5139

#define TX_BUFFER_MID           ((TXSTART) + ((TX_BUFFER_SIZE) >> 1))

//#define FULL_DUPLEX true

//#define LOOP_BACK true

#endif