import logging
import random
import threading
import time
import timeit
import multiprocessing as mp
from queue import Queue
import asyncio

from Worker import Worker

logging.basicConfig(level=logging.INFO)

#from Worker import Worker


def unit_measurements(iter_num):
    total_time = 0
    for i in range(iter_num):
        start = time.time()
        with open('text_file.txt', 'w') as fw:
            fw.write("Mikołaj, 184865")
        with open("text_file.txt", "r") as fr:
            fr.read()
        total_time += time.time() - start
    print(f'Average time with RTC clock: {total_time / iter_num}')
    total_time = 0
    for i in range(iter_num):
        start = time.perf_counter()
        with open('text_file.txt', 'w') as fw:
            fw.write("Mikołaj, 184865")
        with open("text_file.txt", "r") as fr:
            fr.read()
        total_time += time.perf_counter() - start
    print(f'Average time with internal CPU clock: {total_time / iter_num}')
    return


def linear_search(my_list, find):
    for x in my_list:
        if x == find:
            return True
    return False


def is_name_index(name: str, index: int):
    linear_search(str(index), len(name) % 10)


def repeatable_measurements():
    SETUP_CODE = '''from __main__ import linear_search\nfrom __main__ import is_name_index'''
    TEST_CODE = '''with open('text_file.txt', 'w') as fw:\n\tfw.write("Mikołaj, 184865")\nwith open('text_file.txt', 'r') as fr:\n\t
    my_list = (fr.read()).split(', ')\nis_name_index(my_list[0], my_list[1])'''
    NUMBER = 10000
    total_time = timeit.timeit(setup=SETUP_CODE, stmt=TEST_CODE, number=NUMBER)
    print(f'Average time with timeit module and additional function: {total_time / NUMBER}')


def how_many_within_range(row, minimum=2, maximum=4):
    """Returns how many numbers lie within `maximum` and `minimum` in a given `row`"""
    count = 0
    for n in row:
        if minimum <= n <= maximum:
            count = count + 1
    return count


def multiprocessing():
    print(f'Number of logical processors: {mp.cpu_count()}')
    data = [[2, 1, 4, 2, 7, 0], [3, 2, 5, 3, 8, 1], [1, 0, 3, 1, 6, -1], [4, 3, 6, 4, 9, 2]]
    pool = mp.Pool(mp.cpu_count())
    # result_objects = [pool.apply_async(how_many_within_range, args=(row, 2, 4)) for row in data]
    result_objects = [pool.map_async(how_many_within_range, data)]
    results = [r.get() for r in result_objects]
    print(results)
    pool.close()
    pool.join()


def print_cube(num):
    print(f"Cube: {num * num * num}")


def job(worker):
    with open('messages.txt') as f:
        for line in f:
            print(line)


def reader(q):
    while True:
        worker = q.get()
        job(worker)
        q.task_done()


def multithreading():
    # t1 = threading.Thread(target=print_cube, args=(10,))
    # t1.start()
    # t1.join()
    #q = Queue()
    # for x in range(10):
    #     t = threading.Thread(target=reader, args=(q,))
    #     t.daemon = True
    #     t.start()
    # workers = list()
    # for i in range(10):
    #     workers.append(Worker(i, i))
    #     workers[i].run()
    #     workers[i].daemon = True
    #     q.put(workers[i])
    #
    # #time.sleep(5)
    # for worker in workers:
    #     worker.side_task.join()
    # q.join()
    q = Queue()
    workers = []
    for x in range(10):
        workers.append(Worker(x, x, q))
        t = workers[x]
        t.daemon = True
        t.start()
    #time.sleep(3)
    for i, worker in enumerate(workers):
        print(i)
        q.put(i)

    q.join()


num_word_mapping = {1: 'ONE', 2: 'TWO', 3: "THREE", 4: "FOUR", 5: "FIVE", 6: "SIX", 7: "SEVEN", 8: "EIGHT",
                    9: "NINE", 10: "TEN"}


async def delay_message(delay, message):
    logging.info(f"{message} received")
    await asyncio.sleep(delay)
    # time.sleep is blocking call. Hence, it cannot be awaited and we have to use asyncio.sleep
    logging.info(f"Printing {message}")


async def async_main():
    logging.info("Main started")
    logging.info(f'Current registered tasks: {len(asyncio.all_tasks())}')
    logging.info("Creating tasks")
    # task_1 = asyncio.create_task(delay_message(seconds, num_word_mapping[seconds]))
    # seconds = random.randrange(5)
    # task_2 = asyncio.create_task(delay_message(seconds, num_word_mapping[seconds]))
    tasks = []
    for i in range(100):
        seconds = random.randrange(1, 5)
        tasks.append(asyncio.create_task(delay_message(seconds, num_word_mapping[seconds])))
    logging.info(f'Current registered tasks: {len(asyncio.all_tasks())}')
    await asyncio.gather(*tasks)
    # await task_1  # suspends execution of coroutine and gives control back to event loop while awaiting task completion.
    # await task_2
    logging.info("Main Ended")


def main():
    #unit_measurements(100)
    #repeatable_measurements()
    #multiprocessing()
    multithreading()
    pass

if __name__ == '__main__':
    main()
    #asyncio.run(async_main())

