import mpi4py.MPI as mpi

rank, size = mpi.COMM_WORLD.rank, mpi.COMM_WORLD.size

print("hello world from process ", rank,"of", size)