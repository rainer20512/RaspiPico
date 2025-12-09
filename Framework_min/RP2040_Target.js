/*****************************************************************************
 * Copyright (c) 2021 Rowley Associates Limited.                             *
 *                                                                           *
 * This file may be distributed under the terms of the License Agreement     *
 * provided with this software.                                              *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *                                                                           *
 *****************************************************************************/

function IsArmdInterface()
{
  var implementation = TargetInterface.implementation();
  return implementation == "cmsis-dap" || implementation == "crossworks_hardware";
}

function Connect()
{
  if (IsArmdInterface())
    TargetInterface.setDebugInterfaceProperty("TARGETSEL", TargetInterface.getProjectProperty("Target") == "RP2040_M0_1" ? 0x11002927 : 0x01002927);
}

function Reset()
{
  if (IsArmdInterface())
    {
      TargetInterface.pokeWord(0xE000EDFC, TargetInterface.peekWord(0xE000EDFC) | 1); // set VC_CORERESET in DEMCR
      TargetInterface.pokeWord(0xE000ED0C, 0x05FA0004); // set SYSRESETREQ in AIRCR
      timeout = 100;
      while ((TargetInterface.peekWord(0xE000EDF0) & (1 << 25)) == 0) // wait for S_RESET_ST bit in DHCSR to go high
        {
          if (timeout-- == 0)
            TargetInterface.error("reset timed out (S_RESET_ST low)");
          TargetInterface.delay(1);
        }
      timeout = 100;
      while ((TargetInterface.peekWord(0xE000EDF0) & (1 << 25)) == (1 << 25)) // wait for S_RESET_ST bit in DHCSR to go low
        {
          if (timeout-- == 0)
            TargetInterface.error("reset timed out (S_RESET_ST high)");
          TargetInterface.delay(1);
        }
      TargetInterface.waitForDebugState(1000);
      TargetInterface.pokeWord(0xE000EDFC, TargetInterface.peekWord(0xE000EDFC) & ~1); // clear VC_CORERESET in DEMCR
      if (TargetInterface.getProjectProperty("Target") == "RP2040_M0_0" && (TargetInterface.peekWord(0x40004014) & 0x88) == 0x08)
        {
          // Spare USBCTRL, SYSCFG, PLL_USB, and PLL_SYS when RP2040_M0_1 is debugging RP2040_M0_0
          TargetInterface.pokeWord(0x4000c000, 0xFEFBCFFF); // RESET
        }
      else
        {
          TargetInterface.pokeWord(0x4000803c, 0x00000000); // CLK_SYS_CTRL
          TargetInterface.pokeWord(0x4000c001, 0xFFFFFFFF); // RESET
        }
    }
  else
    TargetInterface.resetAndStop(1000);
}

function LinkPatch(filename)
{
  backupFilename = filename + ".prepatch";
  CWSys.removeFile(backupFilename);
  CWSys.renameFile(filename, backupFilename);
  ElfFile.load(backupFilename);
  ElfFile.pokeUint32(0x100000FC, ~ElfFile.crc32(0x10000000, 0xFC, false, 0x00));
  ElfFile.save(filename);
}

function EnableTrace()
{
}

function GetPartName()
{
  var magic = TargetInterface.peekWord(0x00000010);
  if ((magic & 0x00FFFFFF) == 0x0001754D)
    {
      var cpuid = TargetInterface.peekWord(0xD0000000);
      if (cpuid == 0)
        return "RP2040_M0_0";
      if (cpuid == 1)
        return "RP2040_M0_1";
    }
  return "";
}

function MatchPartName(name)
{
  return name == GetPartName();
}

