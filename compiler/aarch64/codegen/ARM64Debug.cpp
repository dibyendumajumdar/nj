/*******************************************************************************
 * Copyright (c) 2018, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include <stdint.h>
#include <string.h>

#include "ras/Debug.hpp"

#include "codegen/ARM64ConditionCode.hpp"
#include "codegen/ARM64Instruction.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/MemoryReference.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/Register.hpp"
#include "codegen/RegisterConstants.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/RegisterDependencyStruct.hpp"
#include "env/IO.hpp"
#include "il/Block.hpp"

static const char *ARM64ConditionNames[] =
   {
   "eq",
   "ne",
   "cs",
   "cc",
   "mi",
   "pl",
   "vs",
   "vc",
   "hi",
   "ls",
   "ge",
   "lt",
   "gt",
   "le",
   "al",
   "nv"
   };

static const char *ARM64ShiftCodeNames[] =
   {
   "lsl",
   "lsr",
   "asr",
   "???"
   };

static const char *ARM64ExtendCodeNames[] =
   {
   "uxtb",
   "uxth",
   "uxtw",
   "uxtx",
   "sxtb",
   "sxth",
   "sxtw",
   "sxtx"
   };

static const char *opCodeToNameMap[] =
   {
   "bad",
   "cbzw",
   "cbnzw",
   "cbzx",
   "cbnzx",
   "tbz",
   "tbnz",
   "b_cond",
   "brkarm64",
   "br",
   "blr",
   "ret",
   "b",
   "bl",
   "stxrb",
   "stlxrb",
   "ldxrb",
   "ldaxrb",
   "stlrb",
   "ldarb",
   "stxrh",
   "stlxrh",
   "ldxrh",
   "ldaxrh",
   "stlrh",
   "ldarh",
   "stxrw",
   "stlxrw",
   "stxpw",
   "stlxpw",
   "ldxrw",
   "ldaxrw",
   "ldxpw",
   "ldaxpw",
   "stlrw",
   "ldarw",
   "stxrx",
   "stlxrx",
   "stxpx",
   "stlxpx",
   "ldxrx",
   "ldaxrx",
   "ldxpx",
   "ldaxpx",
   "stlrx",
   "ldarx",
   "ldrw",
   "vldrs",
   "ldrx",
   "vldrd",
   "ldrsw",
   "vldrq",
   "prfm",
   "stnpw",
   "ldnpw",
   "vstnps",
   "vldnps",
   "vstnpd",
   "vldnpd",
   "stnpx",
   "ldnpx",
   "vstnpq",
   "vldnpq",
   "stppostw",
   "ldppostw",
   "vstpposts",
   "vldpposts",
   "ldpswpost",
   "vstppostd",
   "vldppostd",
   "stppostx",
   "ldppostx",
   "vstppostq",
   "vldppostq",
   "stpoffw",
   "ldpoffw",
   "vstpoffs",
   "vldpoffs",
   "ldpswoff",
   "vstpoffd",
   "vldpoffd",
   "stpoffx",
   "ldpoffx",
   "vstpoffq",
   "vldpoffq",
   "stpprew",
   "ldpprew",
   "vstppres",
   "vldppres",
   "ldpswpre",
   "vstppred",
   "vldppred",
   "stpprex",
   "ldpprex",
   "vstppreq",
   "vldppreq",
   "sturb",
   "ldurb",
   "ldursbx",
   "ldursbw",
   "vsturb",
   "vldurb",
   "vsturq",
   "vldurq",
   "sturh",
   "ldurh",
   "ldurshx",
   "ldurshw",
   "vsturh",
   "vldurh",
   "sturw",
   "ldurw",
   "ldursw",
   "vsturs",
   "vldurs",
   "sturx",
   "ldurx",
   "prfum",
   "vsturd",
   "vldurd",
   "strbpost",
   "ldrbpost",
   "ldrsbpostx",
   "ldrsbpostw",
   "vstrpostb",
   "vldrpostb",
   "vstrpostq",
   "vldrpostq",
   "strhpost",
   "ldrhpost",
   "ldrshpostx",
   "ldrshpostw",
   "vstrposth",
   "vldrposth",
   "strpostw",
   "ldrpostw",
   "ldrswpost",
   "vstrposts",
   "vldrposts",
   "strpostx",
   "ldrpostx",
   "vstrpostd",
   "vldrpostd",
   "sttrb",
   "ldtrb",
   "ldtrsbx",
   "ldtrsbw",
   "sttrh",
   "ldtrh",
   "ldtrshx",
   "ldtrshw",
   "sttrw",
   "ldtrw",
   "ldtrsw",
   "sttrx",
   "ldtrx",
   "strbpre",
   "ldrbpre",
   "ldrsbprex",
   "ldrsbprew",
   "vstrpreb",
   "vldrpreb",
   "vstrpreq",
   "vldrpreq",
   "strhpre",
   "ldrhpre",
   "ldrshprex",
   "ldrshprew",
   "vstrpreh",
   "vldrpreh",
   "strprew",
   "ldrprew",
   "ldrswpre",
   "vstrpres",
   "vldrpres",
   "strprex",
   "ldrprex",
   "vstrpred",
   "vldrpred",
   "strboff",
   "ldrboff",
   "ldrsboffx",
   "ldrsboffw",
   "vstroffb",
   "vldroffb",
   "vstroffq",
   "vldroffq",
   "strhoff",
   "ldrhoff",
   "ldrshoffx",
   "ldrshoffw",
   "vstroffh",
   "vldroffh",
   "stroffw",
   "ldroffw",
   "ldrswoff",
   "vstroffs",
   "vldroffs",
   "stroffx",
   "ldroffx",
   "vstroffd",
   "vldroffd",
   "prfmoff",
   "strbimm",
   "ldrbimm",
   "ldrsbimmx",
   "ldrsbimmw",
   "vstrimmb",
   "vldrimmb",
   "vstrimmq",
   "vldrimmq",
   "strhimm",
   "ldrhimm",
   "ldrshimmx",
   "ldrshimmw",
   "vstrimmh",
   "vldrimmh",
   "strimmw",
   "ldrimmw",
   "ldrswimm",
   "vstrimms",
   "vldrimms",
   "strimmx",
   "ldrimmx",
   "vstrimmd",
   "vldrimmd",
   "prfmimm",
   "adr",
   "adrp",
   "addimmw",
   "addsimmw",
   "subimmw",
   "subsimmw",
   "addimmx",
   "addsimmx",
   "subimmx",
   "subsimmx",
   "andimmw",
   "orrimmw",
   "eorimmw",
   "andsimmw",
   "andimmx",
   "orrimmx",
   "eorimmx",
   "andsimmx",
   "movnw",
   "movzw",
   "movkw",
   "movnx",
   "movzx",
   "movkx",
   "sbfmw",
   "bfmw",
   "ubfmw",
   "sbfmx",
   "bfmx",
   "ubfmx",
   "extrw",
   "extrx",
   "andw",
   "bicw",
   "orrw",
   "ornw",
   "eorw",
   "eonw",
   "andsw",
   "bicsw",
   "andx",
   "bicx",
   "orrx",
   "ornx",
   "eorx",
   "eonx",
   "andsx",
   "bicsx",
   "addw",
   "addsw",
   "subw",
   "subsw",
   "addx",
   "addsx",
   "subx",
   "subsx",
   "addextw",
   "addsextw",
   "subextw",
   "subsextw",
   "addextx",
   "addsextx",
   "subextx",
   "subsextx",
   "adcw",
   "adcsw",
   "sbcw",
   "sbcsw",
   "adcx",
   "adcsx",
   "sbcx",
   "sbcsx",
   "ccmnw",
   "ccmnx",
   "ccmpw",
   "ccmpx",
   "ccmnimmw",
   "ccmnimmx",
   "ccmpimmw",
   "ccmpimmx",
   "cselw",
   "csincw",
   "csinvw",
   "csnegw",
   "cselx",
   "csincx",
   "csinvx",
   "csnegx",
   "maddw",
   "maddx",
   "smaddl",
   "umaddl",
   "msubw",
   "msubx",
   "smsubl",
   "umsubl",
   "smulh",
   "umulh",
   "crc32x",
   "crc32cx",
   "crc32b",
   "crc32cb",
   "crc32h",
   "crc32ch",
   "crc32w",
   "crc32cw",
   "udivw",
   "udivx",
   "sdivw",
   "sdivx",
   "lslvw",
   "lslvx",
   "lsrvw",
   "lsrvx",
   "asrvw",
   "asrvx",
   "rorvw",
   "rorvx",
   "rbitw",
   "rbitx",
   "clzw",
   "clzx",
   "clsw",
   "clsx",
   "revw",
   "revx",
   "rev16w",
   "rev16x",
   "rev32",
   "fmov_stow",
   "fmov_wtos",
   "fmov_dtox",
   "fmov_xtod",
   "fcvt_stod",
   "fcvt_dtos",
   "fcvtzs_stow",
   "fcvtzs_dtow",
   "fcvtzs_stox",
   "fcvtzs_dtox",
   "scvtf_wtos",
   "scvtf_wtod",
   "scvtf_xtos",
   "scvtf_xtod",
   "fmovimms",
   "fmovimmd",
   "fcmps",
   "fcmps_zero",
   "fcmpd",
   "fcmpd_zero",
   "fmovs",
   "fmovd",
   "fabss",
   "fabsd",
   "fnegs",
   "fnegd",
   "fadds",
   "faddd",
   "fsubs",
   "fsubd",
   "fmuls",
   "fmuld",
   "fdivs",
   "fdivd",
   "fmaxs",
   "fmaxd",
   "fmins",
   "fmind",
   "proc",
   "fence",
   "return",
   "dd",
   "label"
   };

const char *
TR_Debug::getOpCodeName(TR::InstOpCode *opCode)
   {
   return opCodeToNameMap[opCode->getMnemonic()];
   }

void
TR_Debug::printMemoryReferenceComment(TR::FILE *pOutFile, TR::MemoryReference *mr)
   {
   if (pOutFile == NULL)
      return;

   TR::Symbol *symbol = mr->getSymbolReference()->getSymbol();
   if (symbol == NULL && mr->getSymbolReference()->getOffset() == 0)
      return;

   trfprintf(pOutFile, "\t\t# SymRef");
   print(pOutFile, mr->getSymbolReference());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::Instruction *instr)
   {
   if (pOutFile == NULL)
      return;

   switch (instr->getKind())
      {
      case OMR::Instruction::IsImm:
         print(pOutFile, (TR::ARM64ImmInstruction *)instr);
         break;
      case OMR::Instruction::IsImmSym:
         print(pOutFile, (TR::ARM64ImmSymInstruction *)instr);
         break;
      case OMR::Instruction::IsLabel:
         print(pOutFile, (TR::ARM64LabelInstruction *)instr);
         break;
      case OMR::Instruction::IsConditionalBranch:
         print(pOutFile, (TR::ARM64ConditionalBranchInstruction *)instr);
         break;
      case OMR::Instruction::IsCompareBranch:
         print(pOutFile, (TR::ARM64CompareBranchInstruction *)instr);
         break;
      case OMR::Instruction::IsRegBranch:
         print(pOutFile, (TR::ARM64RegBranchInstruction *)instr);
         break;
      case OMR::Instruction::IsAdmin:
         print(pOutFile, (TR::ARM64AdminInstruction *)instr);
         break;
      case OMR::Instruction::IsTrg1:
         print(pOutFile, (TR::ARM64Trg1Instruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Cond:
         print(pOutFile, (TR::ARM64Trg1CondInstruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Imm:
         print(pOutFile, (TR::ARM64Trg1ImmInstruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Src1:
         print(pOutFile, (TR::ARM64Trg1Src1Instruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Src1Imm:
         print(pOutFile, (TR::ARM64Trg1Src1ImmInstruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Src2:
         print(pOutFile, (TR::ARM64Trg1Src2Instruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Src2Shifted:
         print(pOutFile, (TR::ARM64Trg1Src2ShiftedInstruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Src2Extended:
         print(pOutFile, (TR::ARM64Trg1Src2ExtendedInstruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Src3:
         print(pOutFile, (TR::ARM64Trg1Src3Instruction *)instr);
         break;
      case OMR::Instruction::IsTrg1Mem:
         print(pOutFile, (TR::ARM64Trg1MemInstruction *)instr);
         break;
      case OMR::Instruction::IsMem:
         print(pOutFile, (TR::ARM64MemInstruction *)instr);
         break;
      case OMR::Instruction::IsMemSrc1:
         print(pOutFile, (TR::ARM64MemSrc1Instruction *)instr);
         break;
      case OMR::Instruction::IsSrc1:
         print(pOutFile, (TR::ARM64Src1Instruction *)instr);
         break;
      case OMR::Instruction::IsSrc2:
         print(pOutFile, (TR::ARM64Src2Instruction *)instr);
         break;
      default:
         TR_ASSERT(false, "unexpected instruction kind");
            // fall through
      case OMR::Instruction::IsNotExtended:
         {
         printPrefix(pOutFile, instr);
         trfprintf(pOutFile, "%s", getOpCodeName(&instr->getOpCode()));
         trfflush(_comp->getOutFile());
         }
      }
   }

void
TR_Debug::printInstructionComment(TR::FILE *pOutFile, int32_t tabStops, TR::Instruction *instr)
   {
   while (tabStops-- > 0)
      trfprintf(pOutFile, "\t");

   dumpInstructionComments(pOutFile, instr);
   }

void
TR_Debug::printPrefix(TR::FILE *pOutFile, TR::Instruction *instr)
   {
   if (pOutFile == NULL)
      return;

   printPrefix(pOutFile, instr, instr->getBinaryEncoding(), instr->getBinaryLength());
   TR::Node *node = instr->getNode();
   trfprintf(pOutFile, "%d \t", node ? node->getByteCodeIndex() : 0);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64ImmInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t0x%08x", getOpCodeName(&instr->getOpCode()), instr->getSourceImmediate());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64ImmSymInstruction *instr)
   {
   printPrefix(pOutFile, instr);

   TR::Symbol *target = instr->getSymbolReference()->getSymbol();
   const char *name = target ? getName(instr->getSymbolReference()) : 0;
   if (name)
      trfprintf(pOutFile, "%s \t" POINTER_PRINTF_FORMAT "\t\t; Direct Call \"%s\"", getOpCodeName(&instr->getOpCode()), instr->getAddrImmediate(), name);
   else
      trfprintf(pOutFile, "%s \t" POINTER_PRINTF_FORMAT, getOpCodeName(&instr->getOpCode()), instr->getAddrImmediate());

   if (instr->getDependencyConditions())
      print(pOutFile, instr->getDependencyConditions());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64LabelInstruction *instr)
   {
   printPrefix(pOutFile, instr);

   TR::LabelSymbol *label = instr->getLabelSymbol();
   TR::Snippet *snippet = label ? label->getSnippet() : NULL;
   if (instr->getOpCodeValue() == TR::InstOpCode::label)
      {
      print(pOutFile, label);
      trfprintf(pOutFile, ":");
      if (label->isStartInternalControlFlow())
         trfprintf(pOutFile, "\t; (Start of internal control flow)");
      else if (label->isEndInternalControlFlow())
         trfprintf(pOutFile, "\t; (End of internal control flow)");
      }
   else
      {
      trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
      print(pOutFile, label);
      if (snippet)
         {
         TR_ASSERT(false, "Not implemented yet.");
         }
      }
   printInstructionComment(pOutFile, 1, instr);
   if (instr->getDependencyConditions())
      print(pOutFile, instr->getDependencyConditions());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64ConditionalBranchInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   TR_ASSERT(instr->getOpCodeValue() == TR::InstOpCode::b_cond, "Unsupported instruction for conditional branch");
   trfprintf(pOutFile, "b.%s \t", ARM64ConditionNames[instr->getConditionCode()]);
   print(pOutFile, instr->getLabelSymbol());
   if (instr->getDependencyConditions())
      print(pOutFile, instr->getDependencyConditions());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64CompareBranchInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   TR_ASSERT(false, "Not implemented yet.");
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64RegBranchInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
   print(pOutFile, instr->getTargetRegister(), TR_DoubleWordReg);
   if (instr->getDependencyConditions())
      print(pOutFile, instr->getDependencyConditions());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64AdminInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s ", getOpCodeName(&instr->getOpCode()));

   TR::Node *node = instr->getNode();
   if (node)
      {
      if (node->getOpCodeValue() == TR::BBStart)
         {
         trfprintf(pOutFile, " (BBStart (block_%d))", node->getBlock()->getNumber());
         }
      else if (node->getOpCodeValue() == TR::BBEnd)
         {
         trfprintf(pOutFile, " (BBEnd (block_%d))", node->getBlock()->getNumber());
         }
      }

   if (instr->getDependencyConditions())
      print(pOutFile, instr->getDependencyConditions());

   trfflush(pOutFile);
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1Instruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
   print(pOutFile, instr->getTargetRegister(), TR_WordReg);
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1CondInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   if (instr->getOpCodeValue() == TR::InstOpCode::csincx)
      {
      // cset alias
      trfprintf(pOutFile, "cset \t");
      print(pOutFile, instr->getTargetRegister(), TR_WordReg);
      trfprintf(pOutFile, ", %s", ARM64ConditionNames[cc_invert(instr->getConditionCode())]);
      }
   else
      {
      trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
      print(pOutFile, instr->getTargetRegister(), TR_WordReg);
      trfprintf(pOutFile, ", xzr, xzr, %s", ARM64ConditionNames[instr->getConditionCode()]);
      }
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1ImmInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
   print(pOutFile, instr->getTargetRegister(), TR_WordReg);
   trfprintf(pOutFile, ", 0x%08x", instr->getSourceImmediate());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1Src1Instruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));

   print(pOutFile, instr->getTargetRegister(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource1Register(), TR_WordReg);
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1Src1ImmInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
   print(pOutFile, instr->getTargetRegister(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource1Register(), TR_WordReg);
   trfprintf(pOutFile, ", %d", instr->getSourceImmediate());

   if (instr->getDependencyConditions())
      print(pOutFile, instr->getDependencyConditions());

   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1Src2Instruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));

   print(pOutFile, instr->getTargetRegister(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource1Register(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource2Register(), TR_WordReg);

   if (instr->getDependencyConditions())
      print(pOutFile, instr->getDependencyConditions());

   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1Src2ShiftedInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));

   print(pOutFile, instr->getTargetRegister(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource1Register(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource2Register(), TR_WordReg);
   trfprintf(pOutFile, " %s %d", ARM64ShiftCodeNames[instr->getShiftType()], instr->getShiftAmount());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1Src2ExtendedInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));

   print(pOutFile, instr->getTargetRegister(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource1Register(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource2Register(), TR_WordReg);
   trfprintf(pOutFile, " %s %d", ARM64ExtendCodeNames[instr->getExtendType()], instr->getShiftAmount());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1Src3Instruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
   print(pOutFile, instr->getTargetRegister(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource1Register(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource2Register(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource3Register(), TR_WordReg);

   if (instr->getDependencyConditions())
      print(pOutFile, instr->getDependencyConditions());

   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Trg1MemInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));

   print(pOutFile, instr->getTargetRegister(), TR_WordReg); trfprintf(pOutFile, ", ");

   print(pOutFile, instr->getMemoryReference());
   TR::Symbol *symbol = instr->getMemoryReference()->getSymbolReference()->getSymbol();
   if (symbol && symbol->isSpillTempAuto())
      {
      trfprintf(pOutFile, "\t\t; spilled for %s", getName(instr->getNode()->getOpCode()));
      }
   if (instr->getSnippetForGC() != NULL)
      {
      trfprintf(pOutFile, "\t\t; Backpatched branch to Unresolved Data %s", getName(instr->getSnippetForGC()->getSnippetLabel()));
      }

   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64MemInstruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
   print(pOutFile, instr->getMemoryReference());
   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64MemSrc1Instruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));

   print(pOutFile, instr->getSource1Register(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getMemoryReference());

   printMemoryReferenceComment(pOutFile, instr->getMemoryReference());
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Src1Instruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
   print(pOutFile, instr->getSource1Register(), TR_WordReg);
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::ARM64Src2Instruction *instr)
   {
   printPrefix(pOutFile, instr);
   trfprintf(pOutFile, "%s \t", getOpCodeName(&instr->getOpCode()));
   print(pOutFile, instr->getSource1Register(), TR_WordReg); trfprintf(pOutFile, ", ");
   print(pOutFile, instr->getSource2Register(), TR_WordReg);

   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::RegisterDependency *dep)
   {
   trfprintf(pOutFile,"[");
   print(pOutFile, dep->getRegister(), TR_WordReg);
   trfprintf(pOutFile," : ");
   trfprintf(pOutFile,"%s] ", getARM64RegisterName(dep->getRealRegister()));
   trfflush(_comp->getOutFile());
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::RegisterDependencyConditions *conditions)
   {
    if (conditions)
      {
      int i;
      trfprintf(pOutFile,"\n PRE: ");
      for (i=0; i<conditions->getAddCursorForPre(); i++)
         {
         print(pOutFile, conditions->getPreConditions()->getRegisterDependency(i));
         }
      trfprintf(pOutFile,"\nPOST: ");
      for (i=0; i<conditions->getAddCursorForPost(); i++)
         {
         print(pOutFile, conditions->getPostConditions()->getRegisterDependency(i));
         }
      trfflush(_comp->getOutFile());
      }
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::MemoryReference *mr)
   {
   trfprintf(pOutFile, "[");

   if (mr->getBaseRegister() != NULL)
      {
      print(pOutFile, mr->getBaseRegister());
      trfprintf(pOutFile, ", ");
      }

   if (mr->getIndexRegister() != NULL)
      print(pOutFile, mr->getIndexRegister());
   else
      trfprintf(pOutFile, "%d", mr->getOffset(true));

   trfprintf(pOutFile, "]");
   }

void
TR_Debug::printARM64GCRegisterMap(TR::FILE *pOutFile, TR::GCRegisterMap *map)
   {
   TR_ASSERT(false, "Not implemented yet.");
   }

void
TR_Debug::print(TR::FILE *pOutFile, TR::RealRegister *reg, TR_RegisterSizes size)
   {
   trfprintf(pOutFile, "%s", getName(reg, size));
   }

static const char *
getRegisterName(TR::RealRegister::RegNum num, bool is64bit)
   {
   switch (num)
      {
      case TR::RealRegister::x0: return (is64bit ? "x0" : "w0");
      case TR::RealRegister::x1: return (is64bit ? "x1" : "w1");
      case TR::RealRegister::x2: return (is64bit ? "x2" : "w2");
      case TR::RealRegister::x3: return (is64bit ? "x3" : "w3");
      case TR::RealRegister::x4: return (is64bit ? "x4" : "w4");
      case TR::RealRegister::x5: return (is64bit ? "x5" : "w5");
      case TR::RealRegister::x6: return (is64bit ? "x6" : "w6");
      case TR::RealRegister::x7: return (is64bit ? "x7" : "w7");
      case TR::RealRegister::x8: return (is64bit ? "x8" : "w8");
      case TR::RealRegister::x9: return (is64bit ? "x9" : "w9");
      case TR::RealRegister::x10: return (is64bit ? "x10" : "w10");
      case TR::RealRegister::x11: return (is64bit ? "x11" : "w11");
      case TR::RealRegister::x12: return (is64bit ? "x12" : "w12");
      case TR::RealRegister::x13: return (is64bit ? "x13" : "w13");
      case TR::RealRegister::x14: return (is64bit ? "x14" : "w14");
      case TR::RealRegister::x15: return (is64bit ? "x15" : "w15");
      case TR::RealRegister::x16: return (is64bit ? "x16" : "w16");
      case TR::RealRegister::x17: return (is64bit ? "x17" : "w17");
      case TR::RealRegister::x18: return (is64bit ? "x18" : "w18");
      case TR::RealRegister::x19: return (is64bit ? "x19" : "w19");
      case TR::RealRegister::x20: return (is64bit ? "x20" : "w20");
      case TR::RealRegister::x21: return (is64bit ? "x21" : "w21");
      case TR::RealRegister::x22: return (is64bit ? "x22" : "w22");
      case TR::RealRegister::x23: return (is64bit ? "x23" : "w23");
      case TR::RealRegister::x24: return (is64bit ? "x24" : "w24");
      case TR::RealRegister::x25: return (is64bit ? "x25" : "w25");
      case TR::RealRegister::x26: return (is64bit ? "x26" : "w26");
      case TR::RealRegister::x27: return (is64bit ? "x27" : "w27");
      case TR::RealRegister::x28: return (is64bit ? "x28" : "w28");
      case TR::RealRegister::x29: return (is64bit ? "x29" : "w29");
      case TR::RealRegister::x30: return "lr";
      case TR::RealRegister::sp: return "sp";
      case TR::RealRegister::xzr: return (is64bit ? "xzr" : "wzr");

      case TR::RealRegister::v0: return (is64bit ? "d0" : "s0");
      case TR::RealRegister::v1: return (is64bit ? "d1" : "s1");
      case TR::RealRegister::v2: return (is64bit ? "d2" : "s2");
      case TR::RealRegister::v3: return (is64bit ? "d3" : "s3");
      case TR::RealRegister::v4: return (is64bit ? "d4" : "s4");
      case TR::RealRegister::v5: return (is64bit ? "d5" : "s5");
      case TR::RealRegister::v6: return (is64bit ? "d6" : "s6");
      case TR::RealRegister::v7: return (is64bit ? "d7" : "s7");
      case TR::RealRegister::v8: return (is64bit ? "d8" : "s8");
      case TR::RealRegister::v9: return (is64bit ? "d9" : "s9");
      case TR::RealRegister::v10: return (is64bit ? "d10" : "s10");
      case TR::RealRegister::v11: return (is64bit ? "d11" : "s11");
      case TR::RealRegister::v12: return (is64bit ? "d12" : "s12");
      case TR::RealRegister::v13: return (is64bit ? "d13" : "s13");
      case TR::RealRegister::v14: return (is64bit ? "d14" : "s14");
      case TR::RealRegister::v15: return (is64bit ? "d15" : "s15");
      case TR::RealRegister::v16: return (is64bit ? "d16" : "s16");
      case TR::RealRegister::v17: return (is64bit ? "d17" : "s17");
      case TR::RealRegister::v18: return (is64bit ? "d18" : "s18");
      case TR::RealRegister::v19: return (is64bit ? "d19" : "s19");
      case TR::RealRegister::v20: return (is64bit ? "d20" : "s20");
      case TR::RealRegister::v21: return (is64bit ? "d21" : "s21");
      case TR::RealRegister::v22: return (is64bit ? "d22" : "s22");
      case TR::RealRegister::v23: return (is64bit ? "d23" : "s23");
      case TR::RealRegister::v24: return (is64bit ? "d24" : "s24");
      case TR::RealRegister::v25: return (is64bit ? "d25" : "s25");
      case TR::RealRegister::v26: return (is64bit ? "d26" : "s26");
      case TR::RealRegister::v27: return (is64bit ? "d27" : "s27");
      case TR::RealRegister::v28: return (is64bit ? "d28" : "s28");
      case TR::RealRegister::v29: return (is64bit ? "d29" : "s29");
      case TR::RealRegister::v30: return (is64bit ? "d30" : "s30");
      case TR::RealRegister::v31: return (is64bit ? "d31" : "s31");

      default: return "???";
      }
   }

const char *
TR_Debug::getName(TR::RealRegister *reg, TR_RegisterSizes size)
   {
   return getRegisterName(reg->getRegisterNumber(), (size == TR_DoubleWordReg || size == TR_DoubleReg));
   }

const char *
TR_Debug::getARM64RegisterName(uint32_t regNum, bool is64bit)
   {
   return getRegisterName((TR::RealRegister::RegNum)regNum, is64bit);
   }

void TR_Debug::printARM64OOLSequences(TR::FILE *pOutFile)
   {
   TR_ASSERT(false, "Not implemented yet.");
   }
