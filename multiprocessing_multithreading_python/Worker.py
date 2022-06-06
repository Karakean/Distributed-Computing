import random
import threading
import time
from threading import Thread


class Worker(Thread):

    def __init__(self, num, ID, q):
        super(Worker, self).__init__()
        self.num = num
        self.ID = ID
        self.q = q
        #self.side_task = threading.Thread(target=self.print_ID, args=(1,))
        #self.side_task.daemon = True

    # def print_ID(self, seconds):
    #     while True:
    #         time.sleep(seconds)
    #         print(f'Task with ID: {self.ID}\n')

    def run(self):
        while True:
            time.sleep(random.random())
            tmp = self.q.get()
            if tmp == self.ID:
                self.q.task_done()
                print(f"Ended task ID: {self.ID}\n")
                #print(str(self.q))
                break
            else:
                self.job(tmp)
                self.q.task_done()
                self.q.put(tmp)

    def job(self, worker_ID):
        time.sleep(1)
        print(f'Thread with ID: {worker_ID}\n')
