/*              
pebble-2fac - A program to calculate and display Google 2-factor
authentication codes on the Pebble smartwatch
Copyright (C) 2014 Martin Pugh

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <pebble.h>
#include "base32.h"
#include "gen_code.h"
#include "hmac.h"
char* get_code(const uint8_t* code, int key_len,unsigned int cur_time,char* out){
    // turn key in to something useful
    uint8_t* code_raw = (uint8_t*)malloc(sizeof(uint8_t)*64);
    int code_raw_len = base32_decode(code,code_raw,64);
    if (code_raw_len == -1){
        strcpy(out,"failed converting code from base32");
        return "Fucked";
    }

    // convert time to correct format
    int step = 30;
    int counter = floor(cur_time/step);
    int i;
    int i_from_right;
    uint8_t* octets = (uint8_t*)malloc(sizeof(uint8_t)*8);
    for (i = 0; i < 8; i++){
        i_from_right = 7-i;
        octets[i_from_right] = counter & 255;
        counter = counter >> 8;
    }
     
    app_log(APP_LOG_LEVEL_DEBUG, "gen_code.c",123,"Time div 30: %x%x%x%x%x%x%x%x",octets[0],octets[1],octets[2],octets[3],octets[4],octets[5],octets[6],octets[7]);
    // perform hmac_sha1
    uint8_t* digest = (uint8_t*)malloc(sizeof(uint8_t)*64);
    if (hmac_sha1(code_raw,code_raw_len,octets,8,digest) != 0){
        out = "failed";
        free(digest);
        free(code_raw);
        free(octets); 
        return "fuck";
    }
    free(code_raw);
    free(octets);
    app_log(APP_LOG_LEVEL_DEBUG, "gen_code.c",123,"Digest: %x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",digest[0],digest[1],digest[2],digest[3],digest[4],digest[5],digest[6],digest[7],digest[8],digest[9],digest[10],digest[11],digest[12],digest[13],digest[14],digest[15],digest[16],digest[17],digest[18],digest[19]);
    // convert to output format
    int offset = digest[19] & 0xf;
    long int bin_code = (digest[offset] & 0x7f) << 24
            |(digest[offset+1] & 0xff) << 16
            |(digest[offset+2] & 0xff) << 8
            |(digest[offset+3] & 0xff);
      
    app_log(APP_LOG_LEVEL_DEBUG, "gen_code.c",123,"Out: %lu\n",bin_code);
    char* long_code = (char*)malloc(sizeof(char)*11);
    snprintf(long_code,16,"%lu",bin_code);
    
    memcpy(out,&long_code[(int)strlen(long_code)-6],7);
    free(digest);
    return out;
}
