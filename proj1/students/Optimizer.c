/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2013                              *
 *  Authors: Ulrich Kremer                   *
 *           Hans Christian Woithe           *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

int main()
{
	Instruction *head;
	Instruction *instr1, *instr2, *instr3;
	int opt_flag, opt_calc;

	head = ReadInstructionList(stdin);
	if (!head) {
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}

	/*If there are less than three instructions then there's no optimization*/
	/*to be done.                                                           */
	if (!head->next->next)
	{
		return EXIT_SUCCESS;
	}

	/*Load the first three manually.*/
	instr1 = head;
	instr2 = head->next;
	instr3 = instr2->next;

	/*temp storage for the result of the optimization*/
	int opt_result;
	while(instr3) {
		if (instr1->opcode == LOADI && instr2->opcode == LOADI)
		{
			if ((instr3->field2 == instr1->field1 &&
				 instr3->field3 == instr2->field1) ||
				(instr3->field2 == instr2->field1 &&
				 instr3->field3 == instr1->field1))
			{
				switch (instr3->opcode)
				{
					case ADD:
						instr1->opcode = LOADI;
						instr1->field1 = instr1->field1 + instr2->field1;
						instr1->next = instr3->next;
						free(instr2);
						free(instr3);
						continue;
						break;
					case MUL:
						instr1->opcode = LOADI;
						instr1->field1 = instr1->field1 * instr2->field1;
						instr1->next = instr3->next;
						free(instr2);
						free(instr3);
						continue;
						break;
					case SUB:
						/*need to check the order...*/
						/*continue;*/
						break;
				}
			}
		}
		instr1=instr2;
		instr2=instr3;
		instr3=instr3->next;
	}

	PrintInstructionList(stdout, head);
	DestroyInstructionList(head);
	return EXIT_SUCCESS;
}
