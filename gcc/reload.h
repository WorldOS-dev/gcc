/* Communication between reload.c and reload1.c.
   Copyright (C) 1987, 91-95, 97, 1998 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


/* If secondary reloads are the same for inputs and outputs, define those
   macros here.  */

#ifdef SECONDARY_RELOAD_CLASS
#define SECONDARY_INPUT_RELOAD_CLASS(CLASS, MODE, X) \
  SECONDARY_RELOAD_CLASS (CLASS, MODE, X)
#define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS, MODE, X) \
  SECONDARY_RELOAD_CLASS (CLASS, MODE, X)
#endif

/* If either macro is defined, show that we need secondary reloads.  */
#if defined(SECONDARY_INPUT_RELOAD_CLASS) || defined(SECONDARY_OUTPUT_RELOAD_CLASS)
#define HAVE_SECONDARY_RELOADS
#endif

/* If MEMORY_MOVE_COST isn't defined, give it a default here.  */
#ifndef MEMORY_MOVE_COST
#ifdef HAVE_SECONDARY_RELOADS
#define MEMORY_MOVE_COST(MODE,CLASS,IN) \
  (4 + memory_move_secondary_cost ((MODE), (CLASS), (IN)))
#else
#define MEMORY_MOVE_COST(MODE,CLASS,IN) 4
#endif
#endif
extern int memory_move_secondary_cost PARAMS ((enum machine_mode, enum reg_class, int));

/* Maximum number of reloads we can need.  */
#define MAX_RELOADS (2 * MAX_RECOG_OPERANDS * (MAX_REGS_PER_ADDRESS + 1))

/* Encode the usage of a reload.  The following codes are supported:

   RELOAD_FOR_INPUT		reload of an input operand
   RELOAD_FOR_OUTPUT		likewise, for output
   RELOAD_FOR_INSN		a reload that must not conflict with anything
				used in the insn, but may conflict with
				something used before or after the insn
   RELOAD_FOR_INPUT_ADDRESS	reload for parts of the address of an object
				that is an input reload
   RELOAD_FOR_INPADDR_ADDRESS	reload needed for RELOAD_FOR_INPUT_ADDRESS
   RELOAD_FOR_OUTPUT_ADDRESS	like RELOAD_FOR INPUT_ADDRESS, for output
   RELOAD_FOR_OUTADDR_ADDRESS	reload needed for RELOAD_FOR_OUTPUT_ADDRESS
   RELOAD_FOR_OPERAND_ADDRESS	reload for the address of a non-reloaded
				operand; these don't conflict with
				any other addresses.
   RELOAD_FOR_OPADDR_ADDR	reload needed for RELOAD_FOR_OPERAND_ADDRESS
                                reloads; usually secondary reloads
   RELOAD_OTHER			none of the above, usually multiple uses
   RELOAD_FOR_OTHER_ADDRESS     reload for part of the address of an input
   				that is marked RELOAD_OTHER.

   This used to be "enum reload_when_needed" but some debuggers have trouble
   with an enum tag and variable of the same name.  */

enum reload_type
{
  RELOAD_FOR_INPUT, RELOAD_FOR_OUTPUT, RELOAD_FOR_INSN, 
  RELOAD_FOR_INPUT_ADDRESS, RELOAD_FOR_INPADDR_ADDRESS,
  RELOAD_FOR_OUTPUT_ADDRESS, RELOAD_FOR_OUTADDR_ADDRESS,
  RELOAD_FOR_OPERAND_ADDRESS, RELOAD_FOR_OPADDR_ADDR,
  RELOAD_OTHER, RELOAD_FOR_OTHER_ADDRESS
};

#ifdef MAX_INSN_CODE
/* Each reload is recorded with a structure like this.  */
struct reload
{
  /* The value to reload from */
  rtx in;
  /* Where to store reload-reg afterward if nec (often the same as
     reload_in)  */
  rtx out;

  /* The class of registers to reload into.  */
  enum reg_class class;

  /* The mode this operand should have when reloaded, on input.  */
  enum machine_mode inmode;
  /* The mode this operand should have when reloaded, on output.  */
  enum machine_mode outmode;

  /* The mode of the reload register.  */
  enum machine_mode mode;

  /* the largest number of registers this reload will require.  */
  int nregs;

  /* Positive amount to increment or decrement by if
     reload_in is a PRE_DEC, PRE_INC, POST_DEC, POST_INC.
     Ignored otherwise (don't assume it is zero).  */
  int inc;
  /* A reg for which reload_in is the equivalent.
     If reload_in is a symbol_ref which came from
     reg_equiv_constant, then this is the pseudo
     which has that symbol_ref as equivalent.  */
  rtx in_reg;
  rtx out_reg;

  /* Used in find_reload_regs to record the allocated register.  */
  int regno;
  /* This is the register to reload into.  If it is zero when `find_reloads'
     returns, you must find a suitable register in the class specified by
     reload_reg_class, and store here an rtx for that register with mode from
     reload_inmode or reload_outmode.  */
  rtx reg_rtx;
  /* The operand number being reloaded.  This is used to group related reloads
     and need not always be equal to the actual operand number in the insn,
     though it current will be; for in-out operands, it is one of the two
     operand numbers.  */
  int opnum;

  /* Gives the reload number of a secondary input reload, when needed;
     otherwise -1.  */
  int secondary_in_reload;
  /* Gives the reload number of a secondary output reload, when needed;
     otherwise -1.  */
  int secondary_out_reload;
  /* If a secondary input reload is required, gives the INSN_CODE that uses the
     secondary reload as a scratch register, or CODE_FOR_nothing if the
     secondary reload register is to be an intermediate register.  */
  enum insn_code secondary_in_icode;
  /* Likewise, for a secondary output reload.  */
  enum insn_code secondary_out_icode;

  /* Classifies reload as needed either for addressing an input reload,
     addressing an output, for addressing a non-reloaded mem ref, or for
     unspecified purposes (i.e., more than one of the above).  */
  enum reload_type when_needed;
  
  /* Nonzero for an optional reload.  Optional reloads are ignored unless the
     value is already sitting in a register.  */
  unsigned int optional:1;
  /* nonzero if this reload shouldn't be combined with another reload.  */
  unsigned int nocombine:1;
  /* Nonzero if this is a secondary register for one or more reloads.  */
  unsigned int secondary_p:1;
  /* Nonzero if this reload must use a register not already allocated to a
     group.  */
  unsigned int nongroup:1;
};

extern struct reload rld[MAX_RELOADS];
extern int n_reloads;
#endif

extern rtx *reg_equiv_constant;
extern rtx *reg_equiv_memory_loc;
extern rtx *reg_equiv_address;
extern rtx *reg_equiv_mem;

/* All the "earlyclobber" operands of the current insn
   are recorded here.  */
extern int n_earlyclobbers;
extern rtx reload_earlyclobbers[MAX_RECOG_OPERANDS];

/* Save the number of operands.  */
extern int reload_n_operands;

/* First uid used by insns created by reload in this function.
   Used in find_equiv_reg.  */
extern int reload_first_uid;

/* Nonzero if indirect addressing is supported when the innermost MEM is
   of the form (MEM (SYMBOL_REF sym)).  It is assumed that the level to
   which these are valid is the same as spill_indirect_levels, above.   */

extern char indirect_symref_ok;

/* Nonzero if an address (plus (reg frame_pointer) (reg ...)) is valid.  */
extern char double_reg_address_ok;

extern int num_not_at_initial_offset;

#ifdef MAX_INSN_CODE
/* These arrays record the insn_code of insns that may be needed to
   perform input and output reloads of special objects.  They provide a
   place to pass a scratch register.  */
extern enum insn_code reload_in_optab[];
extern enum insn_code reload_out_optab[];
#endif

struct needs
{
  /* [0] is normal, [1] is nongroup.  */
  short regs[2][N_REG_CLASSES];
  short groups[N_REG_CLASSES];
};

#if defined SET_HARD_REG_BIT && defined CLEAR_REG_SET
/* This structure describes instructions which are relevant for reload.
   Apart from all regular insns, this also includes CODE_LABELs, since they
   must be examined for register elimination.  */
struct insn_chain 
{
  /* Links to the neighbour instructions.  */
  struct insn_chain *next, *prev;

  /* Link through a chains set up by calculate_needs_all_insns, containing
     all insns that need reloading.  */
  struct insn_chain *next_need_reload;

  /* The basic block this insn is in.  */
  int block;
  /* The rtx of the insn.  */
  rtx insn;
  /* Register life information: record all live hard registers, and all
     live pseudos that have a hard register.  */
  regset_head live_throughout;
  regset_head dead_or_set;

  /* Copies of the global variables computed by find_reloads.  */
  struct reload *rld;
  int n_reloads;

  /* Indicates which registers have already been used for spills.  */
  HARD_REG_SET used_spill_regs;

  /* Describe the needs for reload registers of this insn.  */
  struct needs need;

  /* Nonzero if find_reloads said the insn requires reloading.  */
  unsigned int need_reload:1;
  /* Nonzero if find_reloads needs to be run during reload_as_needed to
     perform modifications on any operands.  */
  unsigned int need_operand_change:1;
  /* Nonzero if eliminate_regs_in_insn said it requires eliminations.  */
  unsigned int need_elim:1;
  /* Nonzero if this insn was inserted by perform_caller_saves.  */
  unsigned int is_caller_save_insn:1;
};

/* A chain of insn_chain structures to describe all non-note insns in
   a function.  */
extern struct insn_chain *reload_insn_chain;

/* Allocate a new insn_chain structure.  */
extern struct insn_chain *new_insn_chain	PARAMS ((void));

extern void compute_use_by_pseudos		PARAMS ((HARD_REG_SET *, regset));
#endif

/* Functions from reload.c:  */

/* Return a memory location that will be used to copy X in mode MODE.  
   If we haven't already made a location for this mode in this insn,
   call find_reloads_address on the location being returned.  */
extern rtx get_secondary_mem PARAMS ((rtx, enum machine_mode,
				    int, enum reload_type));

/* Clear any secondary memory locations we've made.  */
extern void clear_secondary_mem PARAMS ((void));

/* Transfer all replacements that used to be in reload FROM to be in
   reload TO.  */
extern void transfer_replacements PARAMS ((int, int));

/* IN_RTX is the value loaded by a reload that we now decided to inherit,
   or a subpart of it.  If we have any replacements registered for IN_RTX,
   chancel the reloads that were supposed to load them.
   Return non-zero if we chanceled any reloads.  */
extern int remove_address_replacements PARAMS ((rtx in_rtx));

/* Like rtx_equal_p except that it allows a REG and a SUBREG to match
   if they are the same hard reg, and has special hacks for
   autoincrement and autodecrement.  */
extern int operands_match_p PARAMS ((rtx, rtx));

/* Return 1 if altering OP will not modify the value of CLOBBER. */
extern int safe_from_earlyclobber PARAMS ((rtx, rtx));

/* Search the body of INSN for values that need reloading and record them
   with push_reload.  REPLACE nonzero means record also where the values occur
   so that subst_reloads can be used.  */
extern int find_reloads PARAMS ((rtx, int, int, int, short *));

/* Compute the sum of X and Y, making canonicalizations assumed in an
   address, namely: sum constant integers, surround the sum of two
   constants with a CONST, put the constant as the second operand, and
   group the constant on the outermost sum.  */
extern rtx form_sum PARAMS ((rtx, rtx));

/* Substitute into the current INSN the registers into which we have reloaded
   the things that need reloading.  */
extern void subst_reloads PARAMS ((void));

/* Make a copy of any replacements being done into X and move those copies
   to locations in Y, a copy of X.  We only look at the highest level of
   the RTL.  */
extern void copy_replacements PARAMS ((rtx, rtx));

/* Change any replacements being done to *X to be done to *Y */
extern void move_replacements PARAMS ((rtx *x, rtx *y));

/* If LOC was scheduled to be replaced by something, return the replacement.
   Otherwise, return *LOC.  */
extern rtx find_replacement PARAMS ((rtx *));

/* Return nonzero if register in range [REGNO, ENDREGNO)
   appears either explicitly or implicitly in X
   other than being stored into.  */
extern int refers_to_regno_for_reload_p PARAMS ((int, int, rtx, rtx *));

/* Nonzero if modifying X will affect IN.  */
extern int reg_overlap_mentioned_for_reload_p PARAMS ((rtx, rtx));

/* Return nonzero if anything in X contains a MEM.  Look also for pseudo
   registers.  */
extern int refers_to_mem_for_reload_p PARAMS ((rtx));

/* Check the insns before INSN to see if there is a suitable register
   containing the same value as GOAL.  */
extern rtx find_equiv_reg PARAMS ((rtx, rtx, enum reg_class, int, short *,
				 int, enum machine_mode));

/* Return 1 if register REGNO is the subject of a clobber in insn INSN.  */
extern int regno_clobbered_p PARAMS ((int, rtx));

/* Return 1 if X is an operand of an insn that is being earlyclobbered.  */
int earlyclobber_operand_p PARAMS ((rtx));

/* Functions in reload1.c:  */

extern int reloads_conflict		PARAMS ((int, int));

int count_occurrences            PARAMS ((rtx, rtx));

/* Initialize the reload pass once per compilation.  */
extern void init_reload PARAMS ((void));

/* The reload pass itself.  */
extern int reload PARAMS ((rtx, int, FILE *));

/* Mark the slots in regs_ever_live for the hard regs
   used by pseudo-reg number REGNO.  */
extern void mark_home_live PARAMS ((int));

/* Scan X and replace any eliminable registers (such as fp) with a
   replacement (such as sp), plus an offset.  */
extern rtx eliminate_regs PARAMS ((rtx, enum machine_mode, rtx));

/* Emit code to perform a reload from IN (which may be a reload register) to
   OUT (which may also be a reload register).  IN or OUT is from operand
   OPNUM with reload type TYPE.  */
extern rtx gen_reload PARAMS ((rtx, rtx, int, enum reload_type));

/* Deallocate the reload register used by reload number R.  */
extern void deallocate_reload_reg PARAMS ((int r));

/* Functions in caller-save.c:  */

/* Initialize for caller-save.  */
extern void init_caller_save PARAMS ((void));

/* Initialize save areas by showing that we haven't allocated any yet.  */
extern void init_save_areas PARAMS ((void));

/* Allocate save areas for any hard registers that might need saving.  */
extern void setup_save_areas PARAMS ((void));

/* Find the places where hard regs are live across calls and save them.  */
extern void save_call_clobbered_regs PARAMS ((void));

/* Replace (subreg (reg)) with the appropriate (reg) for any operands.  */
extern void cleanup_subreg_operands PARAMS ((rtx));
