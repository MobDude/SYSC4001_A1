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
    std::string interrupt_execution;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace); //split the trace file into activity column and duration column

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if(activity == "CPU"){ //if CPU
            execution += current_time + ", " + duration_intr + ", CPU burst\n"; //add to execution
            current_time+=duration_intr; //increment current time by duration of CPU execution
        }else{ //else its a interupt/systemcall
            [interrupt_execution, current_time] = intr_boilerplate(current_time, duration_intr, context_save_time, activity);
            execution += interrupt_execution; //append
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
