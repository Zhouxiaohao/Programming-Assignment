#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

/*****************By*ZhouHao**********************/                   
/****************The begining*********************/
static int cmd_si(char *args);
static int cmd_info(char *args);
/****************The Ending***********************/ 

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Sinle step debug", cmd_si},
	{ "info", "Print program state information", cmd_info},

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}

/*****************By*ZhouHao**********************/
/****************The begining*********************/

static int string_to_uint(char *str)
{
	int i;
	int res = 0;
	int len = strlen(str);
	for(i = 0;i < len;i ++) {
		if(str[i] < '0' || str[i] > '9') return -1;
		else res = res*10 + (str[i] - '0');
	}
	return res;
}

static int cmd_si(char *args) {
	if(args == NULL) {
		cpu_exec(1);
	}
	else {
		int steps = string_to_uint(args);
//		printf("steps = %d\n", steps);
		if(steps < 0 || !strtok(NULL, " ")) {
			printf("Wrong command 'si' format: si %s\n", args);
			printf("Right command 'si' format: 'si' or 'si + [N]'\n");
		}
		else {
			cpu_exec(steps);
		}
	} 
	
	return 0;
}

static int cmd_info(char *args) {
	if(args == NULL || !strtok(NULL, " ") || (strcmp(args, "r") && strcmp(args, "w"))) {
		printf("Wrong command 'info' format\n");
		printf("Right command 'info' format: 'info r' or 'info w'\n");
	}
	else if(!strcmp(args, "r")) {
		printf("eax\t0x%x\t%d\n", cpu.eax, cpu.eax);
		printf("ecx\t0x%x\t%d\n", cpu.ecx, cpu.ecx);
		printf("edx\t0x%x\t%d\n", cpu.edx, cpu.edx);
		printf("ebx\t0x%x\t%d\n", cpu.ebx, cpu.ebx);
		printf("esp\t0x%x\t%d\n", cpu.esp, cpu.esp);
		printf("ebp\t0x%x\t%d\n", cpu.ebp, cpu.ebp);
		printf("esi\t0x%x\t%d\n", cpu.esi, cpu.esi);
		printf("edi\t0x%x\t%d\n", cpu.edi, cpu.edi);
	}
	else if(!strcmp(args, "w")) {

	}
	else {
		/*Impossible*/
	}

	return 0;
}
/****************The Ending***********************/ 
