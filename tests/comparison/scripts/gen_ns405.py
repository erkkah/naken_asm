#!/usr/bin/env python

import os

def create_asm(instruction):
  out = open("temp.asm", "w")
  out.write("\t" + instruction + "\n\tEND\n")
  out.close()

# --------------------------------- fold here -------------------------------

fp = open("template/ns405.txt", "r")
out = open("ns405.txt", "w")

for instruction in fp:
  instruction = instruction.strip()
  if instruction.startswith(";"): continue
  print(instruction)
  create_asm(instruction.upper())

  # REH NS455 CROSS-ASSEMBLER - VERSION 1.2
  # Running on CP/M 2.2
  os.system("~/bin/iz-cpm ~/bin/xasm455.com temp.asm")

  fp1 = open("TEMP.HEX", "r")

  hex = fp1.readline().strip()

  out.write(instruction + "|" + hex + "\n")

  fp1.close

  os.remove("TEMP.HEX")

fp.close()
out.close()

os.remove("temp.asm")

