#pragma once

#ifndef INC_BAP_H_
#define INC_BAP_H_

static uint16_t bap_header_assemble(uint8_t* buff, int header_point);

static void string_data_to_array(uint8_t* buff, char* data, int len);

void bap_parse(char* data, int len);

#endif