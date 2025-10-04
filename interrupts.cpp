/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * @author Joshua Heinze
 * @author Mark Bowerman
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0; //ms
    int context_save_time = 10; //ms
    int ISR_time = 40;
    std::string interrupt_execution;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace); //split the trace file into activity column and duration column

        /******************ADD YOUR SIMULATION CODE HERE*************************/  //if sysc call - check for errors, if int - check device status
        if(activity == "CPU"){ //if CPU
            execution += std::to_string(current_time) + ", " + duration_intr + ", CPU burst\n"; //add to execution
            current_time+=duration_intr; //increment current time by duration of CPU execution
        }else{ //else its a interupt/systemcall
            [interrupt_execution, current_time] = intr_boilerplate(current_time, duration_intr, context_save_time, vectors);
            execution += interrupt_execution; //append
            execution += std::to_string(current_time) + ", " + std::to_string(ISR_time) + ", " + activity + ": run the ISR (device driver)\n";
            if(activity == "SYSCALL"){
                execution += std::to_string(current_time) + ", " + delays.at(duration_intr) + ", check for errors\n"; //for system call 
            }else{
                execution += std::to_string(current_time) + ", " + delays.at(duration_intr) + ", check device status\n"; //for harware device
            }
            execution += current_time + ", " + std::to_string(1) + ", IRET\n";
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
