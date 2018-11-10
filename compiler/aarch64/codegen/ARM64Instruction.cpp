/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
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

#include "codegen/ARM64Instruction.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/RegisterDependency.hpp"

// TR::ARM64Trg1Instruction:: member functions

bool TR::ARM64Trg1Instruction::refsRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister());
   }

bool TR::ARM64Trg1Instruction::usesRegister(TR::Register *reg)
   {
   return false;
   }

bool TR::ARM64Trg1Instruction::defsRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister());
   }

bool TR::ARM64Trg1Instruction::defsRealRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister()->getAssignedRegister());
   }

void TR::ARM64Trg1Instruction::assignRegisters(TR_RegisterKinds kindToBeAssigned)
   {
   if (getDependencyConditions())
      getDependencyConditions()->assignPostConditionRegisters(this, kindToBeAssigned, cg());

   TR::Machine *machine = cg()->machine();
   TR::Register *targetVirtual = getTargetRegister();
   setTargetRegister(machine->assignOneRegister(this, targetVirtual));

   if (getDependencyConditions())
      getDependencyConditions()->assignPreConditionRegisters(this->getPrev(), kindToBeAssigned, cg());
   }

// TR::ARM64Trg1Src1Instruction:: member functions

bool TR::ARM64Trg1Src1Instruction::refsRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister() ||
           reg == getSource1Register() );
   }

bool TR::ARM64Trg1Src1Instruction::usesRegister(TR::Register *reg)
   {
   return (reg == getSource1Register());
   }

bool TR::ARM64Trg1Src1Instruction::defsRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister());
   }

bool TR::ARM64Trg1Src1Instruction::defsRealRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister()->getAssignedRegister());
   }

void TR::ARM64Trg1Src1Instruction::assignRegisters(TR_RegisterKinds kindToBeAssigned)
   {
   TR::Machine *machine = cg()->machine();
   TR::Register *target1Virtual = getTargetRegister();
   TR::Register *source1Virtual = getSource1Register();

   if (getDependencyConditions())
      getDependencyConditions()->assignPostConditionRegisters(this, kindToBeAssigned, cg());

   target1Virtual->block();
   TR::RealRegister *assignedSource1Register = machine->assignOneRegister(this, source1Virtual);
   target1Virtual->unblock();

   source1Virtual->block();
   TR::RealRegister *assignedTarget1Register = machine->assignOneRegister(this, target1Virtual);
   source1Virtual->unblock();

   if (getDependencyConditions())
      getDependencyConditions()->assignPreConditionRegisters(this->getPrev(), kindToBeAssigned, cg());

   setTargetRegister(assignedTarget1Register);
   setSource1Register(assignedSource1Register);
   }

// TR::ARM64Trg1Src2Instruction:: member functions

bool TR::ARM64Trg1Src2Instruction::refsRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister() ||
           reg == getSource1Register() ||
           reg == getSource2Register());
   }

bool TR::ARM64Trg1Src2Instruction::usesRegister(TR::Register *reg)
   {
   return (reg == getSource1Register() || reg == getSource2Register());
   }

bool TR::ARM64Trg1Src2Instruction::defsRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister());
   }

bool TR::ARM64Trg1Src2Instruction::defsRealRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister()->getAssignedRegister());
   }

void TR::ARM64Trg1Src2Instruction::assignRegisters(TR_RegisterKinds kindToBeAssigned)
   {
   TR::Machine *machine = cg()->machine();
   TR::Register *target1Virtual = getTargetRegister();
   TR::Register *source1Virtual = getSource1Register();
   TR::Register *source2Virtual = getSource2Register();

   if (getDependencyConditions())
      getDependencyConditions()->assignPostConditionRegisters(this, kindToBeAssigned, cg());

   source1Virtual->block();
   target1Virtual->block();
   TR::RealRegister *assignedSource2Register = machine->assignOneRegister(this, source2Virtual);
   target1Virtual->unblock();
   source1Virtual->unblock();

   source2Virtual->block();
   target1Virtual->block();
   TR::RealRegister *assignedSource1Register = machine->assignOneRegister(this, source1Virtual);
   target1Virtual->unblock();
   source2Virtual->unblock();

   source2Virtual->block();
   source1Virtual->block();
   TR::RealRegister *assignedTarget1Register = machine->assignOneRegister(this, target1Virtual);
   source1Virtual->unblock();
   source2Virtual->unblock();

   if (getDependencyConditions())
      getDependencyConditions()->assignPreConditionRegisters(this->getPrev(), kindToBeAssigned, cg());

   setTargetRegister(assignedTarget1Register);
   setSource1Register(assignedSource1Register);
   setSource2Register(assignedSource2Register);
   }

// TR::ARM64MemSrc1Instruction:: member functions

bool TR::ARM64MemSrc1Instruction::refsRegister(TR::Register *reg)
   {
   return (getMemoryReference()->refsRegister(reg) || reg == getSource1Register());
   }

bool TR::ARM64MemSrc1Instruction::usesRegister(TR::Register *reg)
   {
   return (getMemoryReference()->refsRegister(reg) || reg == getSource1Register());
   }

bool TR::ARM64MemSrc1Instruction::defsRegister(TR::Register *reg)
   {
   return false;
   }

bool TR::ARM64MemSrc1Instruction::defsRealRegister(TR::Register *reg)
   {
   return false;
   }

void TR::ARM64MemSrc1Instruction::assignRegisters(TR_RegisterKinds kindToBeAssigned)
   {
   TR::Machine *machine = cg()->machine();
   TR::MemoryReference *mref = getMemoryReference();
   TR::Register *sourceVirtual = getSource1Register();

   if (getDependencyConditions())
      getDependencyConditions()->assignPostConditionRegisters(this, kindToBeAssigned, cg());

   sourceVirtual->block();
   mref->assignRegisters(this, cg());
   sourceVirtual->unblock();

   mref->blockRegisters();
   TR::RealRegister *assignedRegister = sourceVirtual->getAssignedRealRegister();
   if (assignedRegister == NULL)
      {
      assignedRegister = machine->assignOneRegister(this, sourceVirtual);
      }
   mref->unblockRegisters();

   if (sourceVirtual->decFutureUseCount() == 0)
      {
      sourceVirtual->setAssignedRegister(NULL);
      assignedRegister->setState(TR::RealRegister::Unlatched);
      }

   setSource1Register(assignedRegister);

   if (getDependencyConditions())
      getDependencyConditions()->assignPreConditionRegisters(this->getPrev(), kindToBeAssigned, cg());
   }

// TR::ARM64Trg1MemInstruction:: member functions

bool TR::ARM64Trg1MemInstruction::refsRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister() || getMemoryReference()->refsRegister(reg));
   }

bool TR::ARM64Trg1MemInstruction::usesRegister(TR::Register *reg)
   {
   return getMemoryReference()->refsRegister(reg);
   }

bool TR::ARM64Trg1MemInstruction::defsRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister());
   }

bool TR::ARM64Trg1MemInstruction::defsRealRegister(TR::Register *reg)
   {
   return (reg == getTargetRegister()->getAssignedRegister());
   }

void TR::ARM64Trg1MemInstruction::assignRegisters(TR_RegisterKinds kindToBeAssigned)
   {
   TR::Machine *machine = cg()->machine();
   TR::MemoryReference *mref = getMemoryReference();
   TR::Register *targetVirtual = getTargetRegister();

   if (getDependencyConditions())
      getDependencyConditions()->assignPostConditionRegisters(this, kindToBeAssigned, cg());

   mref->blockRegisters();
   setTargetRegister(machine->assignOneRegister(this, targetVirtual));
   mref->unblockRegisters();

   targetVirtual->block();
   mref->assignRegisters(this, cg());
   targetVirtual->unblock();

   if (getDependencyConditions())
      getDependencyConditions()->assignPreConditionRegisters(this->getPrev(), kindToBeAssigned, cg());
   }
