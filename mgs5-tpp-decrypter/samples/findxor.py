import time
from multiprocessing import Pool

#desired_result = 0xDE40D74C # PS3
desired_result = 0xDF92344A # PS4
num_processes = 8  # You can adjust this based on the number of CPU cores you have.

def find_solution(start, end):
    print ("ST=", hex(start), "  END=", hex(end))
    for x in range(start, end):
        num = x
        y = 0
        while y < 5:
            num = (num ^ ((num << 13) & 0xFFFFFFFF)) & 0xFFFFFFFF
            num = (num ^ ((num >> 7) & 0xFFFFFFFF)) & 0xFFFFFFFF
            num = (num ^ ((num << 5) & 0xFFFFFFFF)) & 0xFFFFFFFF
            y += 1

        if (num == desired_result):
            print("X=", hex(x), "   Y=", y)  # Return the solution if found
            return x  # Return the solution if found

    return None  # Return None if no solution found in this range

def main():
    start_time = time.time()

    with Pool(num_processes) as pool:
        # Divide the search space into equal parts for parallel processing
        partition_size = 2**32 // num_processes
        ranges = [(i * partition_size, (i + 1) * partition_size) for i in range(num_processes)]

        results = pool.starmap(find_solution, ranges)

    # Combine the results and print the solution if found
    for result in results:
        if result is not None:
            print("Solution found: x =", result)
            break

    end_time = time.time()
    execution_time = end_time - start_time
    print("Execution time:", execution_time, "seconds")

if __name__ == "__main__":
    main()
