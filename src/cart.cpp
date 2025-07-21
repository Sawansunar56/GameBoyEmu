#include "cart.h"
#include "base_core.h"
#include "base_memory.h"
#include <cstdio>
#include <cstdlib>
#include <array>
#include <cstring>
#include <string>

typedef struct
{
 char filename[1024];
 u32 rom_size;
 u8 *rom_data;
 rom_header *header;

 b8 ram_enabled;
 b8 ram_banking;

 u8 *rom_bank_x;
 u8 banking_mode;

 u8 rom_bank_value;
 u8 ram_bank_value;

 u8 *ram_bank;
 u8 *ram_banks[16];

 b8 battery;
 b8 need_save;
} cart_context;

glob cart_context ctx;

b8 cart_need_save() { return ctx.need_save; }

pure_function b8 cart_mbc1() { return BETWEEN(ctx.header->type, 1, 3); }

pure_function b8 cart_battery() { return ctx.header->type == 3; }

glob const char *ROM_TYPES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

glob const std::array<std::string, 0xA5> LIC_CODE = []() {
 std::array<std::string, 0xA5> arr;

 arr[0x00] = "None";
 arr[0x01] = "Nintendo R&D1";
 arr[0x08] = "Capcom";
 arr[0x13] = "Electronic Arts";
 arr[0x18] = "Hudson Soft";
 arr[0x19] = "b-ai";
 arr[0x20] = "kss";
 arr[0x22] = "pow";
 arr[0x24] = "PCM Complete";
 arr[0x25] = "san-x";
 arr[0x28] = "Kemco Japan";
 arr[0x29] = "seta";
 arr[0x30] = "Viacom";
 arr[0x31] = "Nintendo";
 arr[0x32] = "Bandai";
 arr[0x33] = "Ocean/Acclaim";
 arr[0x34] = "Konami";
 arr[0x35] = "Hector";
 arr[0x37] = "Taito";
 arr[0x38] = "Hudson";
 arr[0x39] = "Banpresto";
 arr[0x41] = "Ubi Soft";
 arr[0x42] = "Atlus";
 arr[0x44] = "Malibu";
 arr[0x46] = "angel";
 arr[0x47] = "Bullet-Proof";
 arr[0x49] = "irem";
 arr[0x50] = "Absolute";
 arr[0x51] = "Acclaim";
 arr[0x52] = "Activision";
 arr[0x53] = "American sammy";
 arr[0x54] = "Konami";
 arr[0x55] = "Hi tech entertainment";
 arr[0x56] = "LJN";
 arr[0x57] = "Matchbox";
 arr[0x58] = "Mattel";
 arr[0x59] = "Milton Bradley";
 arr[0x60] = "Titus";
 arr[0x61] = "Virgin";
 arr[0x64] = "LucasArts";
 arr[0x67] = "Ocean";
 arr[0x69] = "Electronic Arts";
 arr[0x70] = "Infogrames";
 arr[0x71] = "Interplay";
 arr[0x72] = "Broderbund";
 arr[0x73] = "sculptured";
 arr[0x75] = "sci";
 arr[0x78] = "THQ";
 arr[0x79] = "Accolade";
 arr[0x80] = "misawa";
 arr[0x83] = "lozc";
 arr[0x86] = "Tokuma Shoten Intermedia";
 arr[0x87] = "Tsukuda Original";
 arr[0x91] = "Chunsoft";
 arr[0x92] = "Video system";
 arr[0x93] = "Ocean/Acclaim";
 arr[0x95] = "Varie";
 arr[0x96] = "Yonezawa/s’pal";
 arr[0x97] = "Kaneko";
 arr[0x99] = "Pack in soft";
 arr[0xA4] = "Konami (Yu-Gi-Oh!)";

 return arr;
}();

void cart_battery_load()
{
 if (!ctx.ram_bank)
 {
  return;
 }
 char fn[1048];
 sprintf(fn, "%s.battery", ctx.filename);

 FILE *fp = fopen(fn, "rb");

 if (!fp)
 {
  fprintf(stderr, "FAILED TO OPEN FOR LOAD: %s\n", fn);
  return;
 }
 fread(ctx.ram_bank, 0x2000, 1, fp);
 fclose(fp);
}

void cart_battery_save()
{
 if (!ctx.ram_bank)
 {
  return;
 }
 char fn[1048];
 sprintf(fn, "%s.battery", ctx.filename);

 FILE *fp = fopen(fn, "wb");

 if (!fp)
 {
  fprintf(stderr, "FAILED TO OPEN FOR SAVE: %s\n", fn);
  return;
 }

 fwrite(ctx.ram_bank, 0x2000, 1, fp);
 fclose(fp);
}

function const std::string cart_lic_name()
{
 if (ctx.header->new_lic_code <= 0xA4)
 {
  return LIC_CODE[ctx.header->lic_code];
 }

 return "UNKNOWN";
}

function const char *cart_type_name()
{
 if (ctx.header->type <= 0x22)
 {
  return ROM_TYPES[ctx.header->type];
 }

 return "UNKNOWN";
}

function void cart_setup_banking()
{
 for (int i = 0; i < 16; i++)
 {
  ctx.ram_banks[i] = nullptr;

  if ((ctx.header->ram_size == 2 && i == 0) ||
      (ctx.header->ram_size == 3 && i < 4) ||
      (ctx.header->ram_size == 4 && i < 16) ||
      (ctx.header->ram_size == 5 && i < 8))
  {
   ctx.ram_banks[i] = static_cast<u8 *>(malloc(0x2000));
   memset(ctx.ram_banks[i], 0, 0x2000);
  }
 }

 ctx.ram_bank   = ctx.ram_banks[0];
 ctx.rom_bank_x = ctx.rom_data + 0x4000;
}

b8 cart_load(char *cart)
{
 snprintf(ctx.filename, sizeof(ctx.filename), "%s", cart);

 FILE *fp = fopen(cart, "r");

 if (!fp)
 {
  printf("Failed to open: %s\n", cart);
  return false;
 }

 printf("Opened: %s\n", ctx.filename);

 fseek(fp, 0, SEEK_END);
 ctx.rom_size = ftell(fp);

 rewind(fp);

 ctx.rom_data = static_cast<u8 *>(malloc(ctx.rom_size));
 fread(ctx.rom_data, ctx.rom_size, 1, fp);
 fclose(fp);

 ctx.header            = (rom_header *)(ctx.rom_data + 0x100);
 ctx.header->title[15] = 0;
 ctx.battery           = cart_battery();
 ctx.need_save         = false;

 printf("Cartridge Loaded:\n");
 printf("\t Title    : %s\n", ctx.header->title);
 printf("\t Type     : %2.2X (%s)\n", ctx.header->type, cart_type_name());
 printf("\t ROM Size : %d KB\n", 32 << ctx.header->rom_size);
 printf("\t RAM Size : %2.2X\n", ctx.header->ram_size);
 printf("\t LIC Code : %2.2X (%s)\n",
        ctx.header->lic_code,
        cart_lic_name().c_str());
 printf("\t ROM Vers : %2.2X\n", ctx.header->version);

 cart_setup_banking();

 u16 x = 0;
 for (u16 i = 0x0134; i <= 0x014c; i++)
 {
  x = x - ctx.rom_data[i] - 1;
 }

 printf("\t Checksum: %2.2x (%s)\n",
        ctx.header->checksum,
        (x & 0xFF) ? "PASSED" : "FAILED");

 if (ctx.battery)
 {
  cart_battery_load();
 }

 return true;
}

u8 cart_read(u16 address)
{
 if (!cart_mbc1() || address < 0x4000)
 {
  return ctx.rom_data[address];
 }

 if ((address & 0xE000) == 0xA000)
 {
  if (!ctx.ram_enabled)
  {
   return 0xff;
  }

  if (!ctx.ram_bank)
  {
   return 0xff;
  }

  return ctx.ram_bank[address - 0xA000];
 }

 return ctx.rom_bank_x[address - 0x4000];
}

void cart_write(u16 address, u8 value)
{
 if (!cart_mbc1())
 {
  return;
 }

 if (address < 0x2000)
 {
  ctx.ram_enabled = ((value & 0xf) == 0xA);
 }

 if ((address & 0xE000) == 0x2000)
 {
  // rom bank number
  if (value == 0)
  {
   value = 1;
  }

  value &= 0b11111;

  ctx.rom_bank_value = value;
  ctx.rom_bank_x     = ctx.rom_data + (0x4000 * ctx.rom_bank_value);
 }

 if ((address & 0xE000) == 0x4000)
 {
  // ram bank number

  ctx.ram_bank_value = value & 0b11;

  if (ctx.ram_banking)
  {
   if (cart_need_save())
   {
    cart_battery_save();
   }
   ctx.ram_bank = ctx.ram_banks[ctx.ram_bank_value];
  }
 }

 if ((address & 0xE000) == 0x6000)
 {
  // banking mode selected

  ctx.banking_mode = value & 1;

  ctx.ram_banking = ctx.banking_mode;

  if (ctx.ram_banking)
  {
   if (cart_need_save())
   {
    cart_battery_save();
   }
   ctx.ram_bank = ctx.ram_banks[ctx.ram_bank_value];
  }
 }

 if ((address & 0xE000) == 0xA000)
 {
  if (!ctx.ram_enabled)
  {
   return;
  }

  if (!ctx.ram_bank)
  {
   return;
  }

  ctx.ram_bank[address - 0xA000] = value;

  if (ctx.battery)
  {
   ctx.need_save = true;
  }
 }
}
