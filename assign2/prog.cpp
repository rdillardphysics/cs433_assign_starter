/**
 * Assignment 2: Simple UNIX Shell
 * @author Rusty Dillard
 * @brief This is the main function of a simple UNIX Shell. You may add
 * additional functions in this file for your implementation
 * @version 0.1
 */

#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

// The maximum length of a command line
#define MAX_LINE 80

int num_commands = 0;                   	//	Command counter.																								 
int file_index;                         	//	Index of filename in args.
int in, saved_stdin, out, saved_stdout; 	//	Ints for file operations.
int pipe_index;                         	//	Index of pipe in args.
bool should_run = true;                   //	While loop condition.
bool input_redirection_flag = false;    	//	Input redirector found.
bool output_redirection_flag = false;   	//	Output redirector found.
bool pipe_flag = false;                 	//	Pipe found.
bool empty_command = false;             	//	Command entered was empty.
bool check_hist = false;                	//	Check for history.
bool should_wait = true;                	//	Wait found.
char username[4] = {'D', 'a', 'v', 'e'};	//	Username

//	Algorithm to parse a command line entry.
int parse_command(char command[], char *args[]);
//	Set flags to initial program start state.
void reset_flags();
//	Check for and exit command in the user's entry.
bool exit_condition(char **args);
//	Swap the history buffer into the parsing command.
void swap_command(char *history_buffer, char *command, char *parsing_command);
//	Check for ">" or "<" in args.
void check_redirect(char **args);
//	Check for "|" in args.
void check_piping(char **args);
//	Fun boot up thingy.
void booting_up();

/**
 * @brief The main function of a simple UNIX Shell. You may add additional
 * functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[]) {
	// booting_up();	//	Unnecessary, but here for my amusement.
  char command[MAX_LINE];       // The original command that was entered
  char *args[MAX_LINE / 2 + 1]; // Parsed out command line arguments

  pid_t pid;            							 //	Process ID.
  int num_args = 0;     							 //	Number of arguments in command.
  int state_1, state_2; 							 //	States used in piping.
  int pipe_desc[2];										 //	Piping inputs and outputs.
  char *pipe_args_1[MAX_LINE / 2 + 1]; //	Used for piped communication.
  char *pipe_args_2[MAX_LINE / 2 + 1]; //	Used for piped communication.
  char parsing_command[MAX_LINE];      //	Copy of the original command - will feed to parsing_command.
  char history_buffer[MAX_LINE];       //	Copy of the previous command line entry.

  while (should_run) {
    fflush(stdout);
    reset_flags(); //	Reset the flags for the fresh command
    printf("osh>");
    memset(command, 0, MAX_LINE); //	Reset command to all 0s

    fgets(command, MAX_LINE, stdin); // Read the input command

		//	Catches an error of an empty command
    if (strlen(command) <= 0) {
      printf("How did you even do that? Goodbye.");
      continue;
    }

    else {
      //  Copy and parse the input command
      strcpy(parsing_command, command);
      num_args = parse_command(parsing_command, args);

      //	Exit condition
      if (exit_condition(args)) { continue; }

      //	Search for !! command for history
      for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "!!") == 0) {
          check_hist = true;
					
					//	Obvious if statement is obvious.
          if (num_commands < 1) { 
						printf("No command history found.\n"); 
						break;
					}

					//	Swap to the previous command and parse it.
          else {
						swap_command(history_buffer, command, parsing_command);
						printf("%s", parsing_command);
            num_args = parse_command(parsing_command, args);
          }
        }
      }

			//	Go back to looping condition, because user entered a history command with no previous commands.
      if (check_hist && num_commands < 1) { 
				continue; 
			}

			//	Copy the current command into the history buffer for a history call.
      strcpy(history_buffer, command);

			//	Check for redirecting and piping tokens in the parsed arguments.
      check_redirect(args);
      check_piping(args);

      if (input_redirection_flag) {
        //	Open input file with a read only flag
        in = open(args[file_index], O_RDONLY);

				//	File descriptor error.
        if (in < 0) { 
					printf("Failed to open file: %s", args[file_index]); 
					return 1; 
				}
					
        //	File input success.
        else { 
					saved_stdin = dup(0); 
					dup2(in, 0); 
				}
        close(in);
      }

      else if (output_redirection_flag) {
        /*
         *	Open output file with these flags
         *	Create, write only, truncate, and S_IRWXU group flag.
         */
        out = open(args[file_index], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

				//	File descriptor error.
        if (in < 0) { 
					printf("Failed to open file: %s", args[file_index]); 
				}
					
				//	File output success.
        else { 
					saved_stdout = dup(1); 
					dup2(out, 1); 
				}
				
        close(out);
      }

      //  Preconditioning for pipe command.
      if (pipe_flag) {
				//	Search for pipe token in pipe_args_1.
        for (int i = 0; i <= pipe_index; i++) {
					//	When pipe is found, set it to NULL and break the loop.
          if (i == pipe_index) {
            pipe_args_1[i] = NULL;
            break;
          }
					//	Copy the argument at args[i] into pipe_args_1[i].
          pipe_args_1[i] = args[i];
        }

				//	Determines last index of the post-pipe arguments.
        int last_i = 0;

				//	Copy the arguments at args[i], one by one,
				//	starting from the beginning of pipe_args_2
				//	and adjusting for the offset of i.
        for (int i = pipe_index + 1; args[i] != NULL; i++) {
          pipe_args_2[i - pipe_index - 1] = args[i];
          last_i = i;
        }

				//	Set the index after the last argument to NULL
				//	for args flagging in execvp.
        pipe_args_2[last_i + 1] = NULL;
      }

			//	Determine if the command entered was somehow empty.
      if (args[0] == NULL || strcmp(args[0], "\n") == 0 || strcmp(args[0], "\0") == 0) {
        empty_command = true;
      }

      /**
       * After reading user input, the steps are:
       * (1) fork a child process using fork()
       * (2) the child process will invoke execvp()
       * (3) parent will invoke wait() unless command included &
       */
      if (!empty_command) {
        //	Create a child process.
        pid = fork();

        //	Execute if the fork failed for any reason.
        if (pid < 0) {
          fprintf(stderr, "Fork failed.\n");
          continue;
        }

        //	Execute the child process.
        else if (pid == 0) {
          //	Redirection condition
          if (input_redirection_flag || output_redirection_flag) {
            char *args_new[MAX_LINE / 2 + 1];
						//	Add args[i] to args_new[i] until a redirection token is reached.
            for (int i = 0; !(strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0); i++) {
              args_new[i] = args[i];
            }
						//	Execute the command.
            state_1 = execvp(args_new[0], args_new);
            printf("An error occured.\n");
            exit(0);
          }

          //	Piping condition
          else if (pipe_flag) {
						//	New process ID.
            pid_t pid_2;
            pipe(pipe_desc);
            pid_2 = fork();

						//	Failed to fork.
            if (pid_2 < 0) {
              printf("PID returned -1. Fork failed.");
              exit(0);
            }
            //  Create grandchild
            else if (pid_2 == 0) {
              saved_stdout = dup(1);
              //  Replace stdout with the output part of the pipe
              dup2(pipe_desc[1], 1);
              //  Close unused input side of the pipe
              close(pipe_desc[0]);
              //  Enter command to produce output to the other command
              execvp(pipe_args_1[0], pipe_args_1);
              fprintf(stderr, "Command not found.\n");
            }
            //  Run in child of shell
            else if (pid_2 > 0) {
              wait(NULL);
              saved_stdin = dup(0);
              //  Replace stdin with the input part of the pipe
              dup2(pipe_desc[0], 0);
              //  Close output pipe
              close(pipe_desc[1]);
              //  Execute command that will take input from the pipe
              state_2 = execvp(pipe_args_2[0], pipe_args_2);
              if (state_2 < 0) {
                fprintf(stderr, "Command not found.\n");
              }
            }

          }
          //	Normal, print-to-console condition.
          else {
            execvp(args[0], args);
            fprintf(stderr, "Command not found.\n");
            continue;
          }
        }

        //	Finish executing parent process.
        else {
          //	If should_wait returns true, wait for child to finish.
          if (should_wait)
            int wc = wait(NULL);
        }
      }
    }
		
		if (input_redirection_flag || output_redirection_flag || pipe_flag) {
			// Restore default in and out
			dup2(saved_stdout, 1);
			dup2(saved_stdin, 0);
		}
    num_commands++;
  }
  exit(0);
}

/**
 * @brief parse out the command and arguments from the input command separated
 * by spaces
 *
 * @param command: The entire command passed into the function.
 * @param args: An empty array of cstrings to be loaded with the tokenized
 * command.
 * @return int: The number of arguments in the command.
 */
int parse_command(char *command, char **args) {
  int arg_count = 0; 							 		// 	Stores the number of arguments.
	int command_len = strlen(command);	//	Stores the length of the initial command.
  int arg_i = -1;                    	// 	Next argument's starting index.
	
	//	Parse through the entire length of the command line entry.
  for (int i = 0; i < command_len; i++) {
		//	Handling of possible tokens and flags in command line.
    switch (command[i]) {
		//	White space found.
    case ' ':
		case '\t':
			//	Store the string at the address of command[arg_i] into args[arg_count]
      if (arg_i != -1) {
        args[arg_count] = &command[arg_i];	//	Store string at this address into args at arg_count.
        arg_count++;
      }
      command[i] = '\0';
      arg_i = -1;
      break;

		//	End of the entered command found.
    case '\n':
			//	Store the string at the address of command[arg_i] into args[arg_count]
      if (arg_i != -1) {
        args[arg_count] = &command[arg_i];	//	Store string at this address into args at arg_count.
        arg_count++;
      }
      command[i] = '\0';
      break;

		//	
    default:
			//	Update next argument's initial starting index, if not already set.
      if (arg_i == -1)
        arg_i = i;
      if (command[i] == '&') {
        should_wait = false; 	//	Parent will not wait for child process to finish.
        command[i] = '\0';		//	Replace '&' with NULL flag.
        i++;
      }
      break;
    }
  }
	//	Set end of args to NULL and return the num
  args[arg_count] = NULL;
  return arg_count;
}

void reset_flags() {
  should_wait = true;
  input_redirection_flag = false;
  output_redirection_flag = false;
  pipe_flag = false;
  empty_command = false;
  check_hist = false;
	return;
}

bool exit_condition(char **args) {
	//	Search for term "exit" anywhere in their command line.
  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "exit") == 0) {
      should_run = false;
      return true;
    }
  }
  return false;
}

void swap_command(char *history_buffer, char *command, char *parsing_command) {
  strcpy(command, history_buffer);
  strcpy(parsing_command, command);
}

void check_redirect(char **args) {
	//	Search for input or output redirection token. Set appropriate flags.
  for (int i = 0; args[i] != NULL; i++) {
		//	Input token found.
    if (strcmp(args[i], "<") == 0) {
      input_redirection_flag = true;
			
			//	Check if there's a filename after the input redirection token.
      if (args[i + 1] != NULL) {
				file_index = i + 1;
			}
			else { 
				printf("Command %s has an invalid filename.", args[0]);
			}
    }

		//	Output token found.
    if (strcmp(args[i], ">") == 0) {
      output_redirection_flag = true;
			
			//	Check if there's a filename after the input redirection token.
      if (args[i + 1] != NULL) { file_index = i + 1; } 
			else {
				printf("Command %s has an invalid filename.", args[0]);
			}
    }
  }
  return;
}

void check_piping(char **args) {
	//	Search for pipe token. Set appropriate flag.
  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "|") == 0) {
      pipe_flag = true;
      pipe_index = i;
      if (args[i + 1] == NULL) { 
				printf("Invalid command after |. \n"); 
			}
    }
  }
  return;
}

void booting_up() {
	//	Just a nod to 2001: A Space Odyssey.
	printf("Booting HAL-9000");
	int timer = 0;
	while (timer < 3) {
		printf(".");
		sleep(1);
		fflush(stdout);
		timer++;
	}
	timer = strlen(username);
	int c = 0;
	printf("\nuser: ");
	while (timer > 0) {
		printf("%c", username[c]);
		usleep(200000);
		fflush(stdout);
		timer--;
		c++;
	}
	printf("\n");
}

