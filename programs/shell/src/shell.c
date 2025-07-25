#include "shell.h"
#include "stdio/stdio.h"
#include "stdlib/stdlib.h"
#include "peachos.h"
#include "string/string.h"

int main(int argc, char** argv)
{
    static char current_path[512] = "0:/";
    print("PeachOS v1.0.0\n");
    while(1) 
    {
        print(current_path);
        print("> ");
        char buf[1024];
        peachos_terminal_readline(buf, sizeof(buf), true);
        print("\n");

        int res = peachos_user_command_run(buf, current_path);

        if(res < 0){
            int system_res = peachos_system_run(buf);
            if(system_res < 0)
            {
                printf("command not found: %i, system response: %i", res, system_res);
            }
        }
        
        print("\n");
    }
    return 0;
}