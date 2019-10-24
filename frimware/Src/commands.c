#include <stdio.h>

#include "Utils/cli.h"
#include "Utils/commands.h"

void rtc_debug(uint8_t argc, char **argv);
const sTermEntry_t rtcEntry =
{ "date", "RTC date", rtc_debug };

void ir_fan_debug(uint8_t argc, char **argv);
const sTermEntry_t fanEntry =
{ "f", "Start Fan", ir_fan_debug };

void ir_ac_debug(uint8_t argc, char **argv);
const sTermEntry_t acEntry =
{ "a", "Start AC", ir_ac_debug };

void ir_off_debug(uint8_t argc, char **argv);
const sTermEntry_t offEntry =
{ "o", "Switch AC off", ir_off_debug };

void button_debug(uint8_t argc, char **argv);
const sTermEntry_t buttonEntry =
{ "i", "Sample inputs", button_debug };

const sTermEntry_t *cli_entries[] =
{
      &hEntry,
      &helpEntry,
      &rebootEntry,
      &bootEntry,
	  &rtcEntry,
	  &fanEntry,
	  &offEntry,
	  &acEntry,
	  &buttonEntry,
      0
};
