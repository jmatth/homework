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
		printf("Not enough instructions.");
		return EXIT_SUCCESS;
	}

	/*Load the first three manually.*/
	instr1 = head;
	instr2 = head->next;
	instr3 = instr2->next;

	while(instr3) {
		opt_flag=0;
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
						opt_flag = 1;
						opt_calc = instr1->field2 + instr2->field2;
						break;
					case MUL:
						opt_flag = 1;
						opt_calc = instr1->field2 * instr2->field2;
						break;
					case SUB:
						opt_flag = 1;
						if (instr3->field2 == instr1->field2)
						{
							opt_calc = instr1->field2 - instr2->field2;
						}
						else
						{
							opt_calc = instr2->field2 - instr1->field2;
						}
						break;
					default:
						break;
				}
			}
		}
		if (opt_flag)
		{
			instr1->opcode = LOADI;
			instr1->field1 = instr3->field1;
			instr1->field2 = opt_calc;
			instr1->next = instr3->next;
			free(instr2);
			free(instr3);

			if (instr1->prev)
			{
				instr1=instr1->prev;
			}

			instr2=instr1->next;
			instr3=instr2->next;
		}
		else
		{
			instr1=instr2;
			instr2=instr3;
			instr3=instr3->next;
		}
	}

	PrintInstructionList(stdout, head);
	DestroyInstructionList(head);
	return EXIT_SUCCESS;
}
