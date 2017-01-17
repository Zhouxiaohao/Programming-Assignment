#include "cpu/exec/helper.h"

/*
 * see nemu/include/cpu/help.h
 * #define make_helper(name) int name(swaddr_t eip)
 */
make_helper(exec);

make_helper(operand_size) {
	ops_decoded.is_operand_size_16 = true;
	int instr_len = exec(eip + 1);
	ops_decoded.is_operand_size_16 = false;
	return instr_len + 1;
}
