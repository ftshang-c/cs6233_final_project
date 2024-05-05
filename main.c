#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct Process {
    int process_id;
    int execution_remaining;
    int total_execution_time;
} Process;

float shortest_job_scheduler(struct Process * process_list, int size)
{
    for (int i = 0; i < size; i += 1)
    {
        int smallest_index = i;
        for (int j = i; j < size; j += 1)
        {
            if (process_list[smallest_index].execution_remaining > process_list[j].execution_remaining)
            {
                smallest_index = j;
            }
        }
        Process temp = process_list[i];
        process_list[i] = process_list[smallest_index];
        process_list[smallest_index] = temp;
    }

    int time = 0;
    for (int i = 0; i < size; i += 1)
    {
        while (process_list[i].execution_remaining > 0)
        {
            time += 1;
            process_list[i].execution_remaining -= 1;
        }
        process_list[i].total_execution_time = time;
    }

    int sum = 0;
    for (int i = 0; i < size; i += 1)
    {
        sum += process_list[i].total_execution_time;
    }

    return (float) sum / (float) size;
}

float first_come_first_serve_scheduler(struct Process * process_list, int size)
{
    int time = 0;
    for (int i = 0; i < size; i += 1)
    {
        while (process_list[i].execution_remaining > 0)
        {
            time += 1;
            process_list[i].execution_remaining -= 1;
        }
        process_list[i].total_execution_time = time;
    }

    int sum = 0;
    for (int i = 0; i < size; i += 1) {
        sum += process_list[i].total_execution_time;
    }

    return (float) sum / (float) size;
}

bool process_remaining(struct Process * process_list, int size)
{
    for (int i = 0; i < size; i += 1)
    {
        if (process_list[i].execution_remaining > 0)
        {
            return true;
        }
    }
    return false;
}

float round_robin(struct Process * process_list, int size)
{
    int time = 0;
    while (process_remaining(process_list, size))
    {
        for (int i = 0; i < size; i += 1)
        {
            Process current_process = process_list[i];
            if (process_list[i].execution_remaining > 0)
            {
                int time_temp = 0;
                while (time_temp < 20 && process_list[i].execution_remaining > 0)
                {
                    time += 1;
                    time_temp += 1;
                    process_list[i].execution_remaining -= 1;
                }
                if (process_list[i].execution_remaining == 0)
                {
                    process_list[i].total_execution_time = time;
                }
            }
        }
    }

    int sum = 0;
    for (int i = 0; i < size; i += 1)
    {
        sum += process_list[i].total_execution_time;
    }

    return (float) sum / (float) size;
}

int main()
{
    int MAX_NUM_PROCESSES = 4;
    int NUM_SWAPS = 1000;
    int ITERATIONS = 10000;
    int SEED = 1000;
    srand(time(NULL));   // Initialization, should only be called once.

    float fcfs_avg_completion[ITERATIONS];
    float rr_avg_completion[ITERATIONS];
    float sj_avg_completion[ITERATIONS];

    for (int num_iterations = 0; num_iterations < ITERATIONS; num_iterations += 1)
    {
        Process process_block_first_come_first_serve[MAX_NUM_PROCESSES];
        Process process_block_round_robin[MAX_NUM_PROCESSES];
        Process process_block_shortest_job[MAX_NUM_PROCESSES];
        // 1. Randomly generate list of processes.
        for (int i = 0; i < MAX_NUM_PROCESSES; i += 1) {
            int new_execution_time = rand() % 100 + 1;
            Process fcfs_process = {i, new_execution_time, 0};
            Process rr_process = {i, new_execution_time, 0};
            Process sj_process = {i, new_execution_time, 0};
            process_block_first_come_first_serve[i] = fcfs_process;
            process_block_round_robin[i] = rr_process;
            process_block_shortest_job[i] = sj_process;
        }

        // 2. Randomly generate order.
        int num_swaps = rand() % NUM_SWAPS + 1;
        int current_swap = 0;
        while (current_swap < num_swaps)
        {
            int random_index_1 = rand() % MAX_NUM_PROCESSES;
            int random_index_2 = rand() % MAX_NUM_PROCESSES;

            Process fcfs_temp = process_block_first_come_first_serve[random_index_1];
            process_block_first_come_first_serve[random_index_1] = process_block_first_come_first_serve[random_index_2];
            process_block_first_come_first_serve[random_index_2] = fcfs_temp;

            Process rr_temp = process_block_round_robin[random_index_1];
            process_block_round_robin[random_index_1] = process_block_round_robin[random_index_2];
            process_block_round_robin[random_index_2] = rr_temp;

            Process sj_temp = process_block_shortest_job[random_index_1];
            process_block_shortest_job[random_index_1] = process_block_shortest_job[random_index_2];
            process_block_shortest_job[random_index_2] = sj_temp;

            current_swap += 1;
        }

        fcfs_avg_completion[num_iterations] = first_come_first_serve_scheduler(process_block_first_come_first_serve, MAX_NUM_PROCESSES);
        rr_avg_completion[num_iterations] = round_robin(process_block_round_robin, MAX_NUM_PROCESSES);
        sj_avg_completion[num_iterations] = shortest_job_scheduler(process_block_shortest_job, MAX_NUM_PROCESSES);
    }

    float sum_fcfs_ac = 0;
    float sum_rr_ac = 0;
    float sum_sj_ac = 0;

    float throughput_fcfs[ITERATIONS];
    float throughput_rr[ITERATIONS];
    float throughput_sj[ITERATIONS];

    for (int i = 0; i < ITERATIONS; i += 1)
    {
        sum_fcfs_ac += fcfs_avg_completion[i];
        throughput_fcfs[i] = ((float) MAX_NUM_PROCESSES) / fcfs_avg_completion[i];

        sum_rr_ac += rr_avg_completion[i];
        throughput_rr[i] = ((float) MAX_NUM_PROCESSES) / rr_avg_completion[i];

        sum_sj_ac += sj_avg_completion[i];
        throughput_sj[i] = ((float) MAX_NUM_PROCESSES) / sj_avg_completion[i];
    }

    float all_fcfs_avg = sum_fcfs_ac / (float) ITERATIONS;
    float all_rr_avg = sum_rr_ac / (float) ITERATIONS;
    float all_sj_avg = sum_sj_ac / (float) ITERATIONS;

    printf("average completion time FCFS: %.2f seconds\n", all_fcfs_avg);
    printf("average completion time RR: %.2f seconds\n", all_rr_avg);
    printf("average completion time SJ: %.2f seconds\n", all_sj_avg);

    float throughput_sum_fcfs = 0;
    float throughput_sum_rr = 0;
    float throughput_sum_sj = 0;

    // Calculate Throughput
    for (int i = 0; i < ITERATIONS; i += 1)
    {
        throughput_sum_fcfs += throughput_fcfs[i];
        throughput_sum_rr += throughput_rr[i];
        throughput_sum_sj += throughput_sj[i];
    }


    float total_avg_throughput_fcfs = throughput_sum_fcfs / (float) ITERATIONS;
    float total_avg_throughput_rr = throughput_sum_rr / (float) ITERATIONS;
    float total_avg_throughput_sj = throughput_sum_sj / (float) ITERATIONS;

    printf("average FCFS throughput out of %d runs: %.6f processes completed per second\n", ITERATIONS, total_avg_throughput_fcfs);
    printf("average RR throughput out of %d runs: %.6f processes completed per second\n", ITERATIONS, total_avg_throughput_rr);
    printf("average SJ throughput out of %d runs: %.6f processes completed per second\n", ITERATIONS, total_avg_throughput_sj);
    return 0;
}
