import mpi4py.MPI as mpi
import numpy as np

rank, size = mpi.COMM_WORLD.rank, mpi.COMM_WORLD.size
comm = mpi.COMM_WORLD
# randNum = np.zeros(1)

print("hello world from process ", rank,"of", size)

if rank == 1:
    randNum = np.random.randint(3,10,size=(1,))
    comm.send(randNum,dest=0, tag=11)

if rank == 0 :
    randNum = comm.recv(source=1, tag=11)
    print(f'I, process {rank}, just received "{randNum}" ')