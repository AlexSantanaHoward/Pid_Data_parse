#pragma once

#ifndef INC_CAN_IDS_H_
#define INC_CAN_IDS_H_

#define gw_motor        0x5b03
#define gw_ant1         0x6703
#define gw_ant2         0x6903

#define bsg_kombi       0x7004

#define unknown_8       0x9105
#define unknown_5       0x2705
#define unknown_6       0x5505
#define bsg3            0x7505
#define mfa4            0x2905
#define bsg2            0x7105
#define gateway_kombi   0x7205

#define system_info1    0x5106
#define fzg_ident       0x5f06
#define unknown_1       0xd806
#define unknown_9       0xb406

#define mfa1            0x2906
#define mfa2            0x2b06
#define mfa3            0x2d06

#define radio           0x6c06

#define unknown_7       0x5306
#define kombi_k1        0x2106

#define unknown_2       0x6051
#define unknown_3       0x01f0



char* check_id_name(uint16_t can_id);

#endif