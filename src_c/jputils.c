#include "jpextractor.h"

word convert_little_endian(byte a[], int n, int k)
{
    word result = 0;
    for (int i = n; i >= k; i--)
    {
        result = a[i] << 8 * i | result;
    }
    return result;
}

word find_dialogue_section(FILE *f, word count, word *section_size)
{
    byte toc_entry[16];
    word tmp = 0;
    word address = 0x0800;
    do
    {
        address += tmp;
        fread(toc_entry, 1, sizeof(toc_entry), f);

        if (is_dialogue_section(toc_entry))
        {
            *section_size = convert_little_endian(toc_entry, 3, 0);
            return address;
        }

        tmp = convert_little_endian(toc_entry, 3, 0);
        if (tmp % 0x0800 != 0)
        {
            tmp = 0x0800 * ((tmp + 0x0800) / 0x0800);
        }
    }
    while (count--);
    return 0;
}

void load_lookup_table(int n, char table[][n], FILE *source)
{
    int i = 0;
    int j = 0;
    char c;
    while((c = fgetc(source)) != EOF)
    {
        if (c != '\n')
        {
            table[i][j] = c;
            j++;
        }
        else
        {
            table[i][j] = '\0';
            j = 0;
            i++;
        }
    }
}

bool is_math_tbl(byte toc_header[])
{
    byte magic[8] = {0x4d, 0x41, 0x54, 0x48, 0x5f, 0x54, 0x42, 0x4c};
    for (int i = 8; i < 16; i++)
    {
        if(toc_header[i] != magic[i - 8])
        {
            return false;
        }
    }
    return true;
}

bool is_dialogue_section(byte toc_entry[])
{
    return toc_entry[8] == 0x00 && toc_entry[9] == 0x02 && toc_entry[11] == 0x02;
}

bool is_hiragana(byte a)
{
    return a >= HRSTART && a <= HREND;
}

bool is_katakana(byte a)
{
    return a >= KTSTART && a <= KTEND;
}

bool is_kanji_start(byte a)
{
    return a == 0x12 || a == 0x13;
}

char *is_punct(byte a)
{
    switch (a)
    {
        case 0x2A:
            return "「";
            break;
        case 0x3E:
            return "‥";
            break;
        case 0x3F:
            return "？";
            break;
        case 0x40:
            return "！";
            break;
        case 0xFC:
            return "ー";
            break;
        case 0xFD:
            return "。";
            break;
        case 0xFE:
            return "、";
            break;
        default:
            return "";
            break;
    }
}

char *is_color(byte a)
{
    switch (a)
    {
        case 0x01:
            return "PURPLE";
            break;
        case 0x02:
            return "RED";
            break;
        case 0x03:
            return "CYAN";
            break;
        case 0x04:
            return "YELLOW";
            break;
        case 0x05:
            return "PINK";
            break;
        case 0x06:
            return "GREEN";
            break;
        case 0x07:
            return "BLACK";
            break;
        default:
            return "";
            break;            
    }    
}

char *is_effect(byte a)
{
    switch (a)
    {
        case 0x00:
            return "type=shake duration=short";
            break;        
        case 0x01:
            return "type=shake duration=long";
            break;
        case 0x02:
            return "type=shake duration=indef";
            break;
        case 0x03:
            return "type=big1 duration=short";
            break;
        case 0x04:
            return "size=big2 duration=short";
            break;
        case 0x05:
            return "size=big3 duration=short";
            break;
        case 0x06:
            return "size=big1 duration=long";
            break;
        case 0x07:
            return "size=big2 duration=long";
            break;
        case 0x08:
            return "size=big3 duration=long";
            break;
        case 0x09:
            return "size=big1 duration=indef";
            break;
        case 0x0a:
            return "size=big2 duration=indef";
            break;
        case 0x0b:
            return "size=big3 duration=indef";
            break;
        case 0x0c:
            return "size=small1 duration=short";
            break;
        case 0x0d:
            return "size=small2 duration=short";
            break;
        case 0x0e:
            return "size=small3 duration=short";
            break; 
        case 0x0f:
            return "size=small1 duration=long";
            break;
        case 0x10:
            return "size=small2 duration=long";
            break;
        case 0x11:
            return "size=small3 duration=long";
            break;
        case 0x12:
            return "size=small1 duration=indef";
            break;
        case 0x13:
            return "size=small2 duration=indef";
            break;
        case 0x14:
            return "size=small3 duration=indef";
            break;
        case 0x15:
            return "type=rise duration=long";
            break;
        case 0x16:
            return "type=rise duration=indef";
            break;
        case 0x17:
            return "type=jump1";
            break;
        case 0x18:
            return "type=jump2";
            break;
        case 0x19:
            return "type=jump3";
            break;
        default:
            return "";
            break;
    }    
}
