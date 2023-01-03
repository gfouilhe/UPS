import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import convolve2d, wiener 
from scipy.fft import fft2, ifft2
from skimage.restoration import wiener as SKwiener

i = plt.imread('lena_flou_bruite.png')

plt.imshow(i,cmap='gray')
plt.title('$i$')
plt.show()

h = 1/(15**2)*np.ones((15,15))

M,N  = i.shape

I = fft2(i)
H = fft2(h, s=i.shape)


wiener_I = np.zeros_like(I)

K = 0.03
for m in range(M):
    for n in range(N):
        Hmn = H[m,n]
        wiener_I[m,n] = I[m,n]*np.conj(Hmn)/(abs(Hmn)**2+K)

plt.imshow(np.real(ifft2(wiener_I)),cmap='gray')
plt.title('$i_{bruitée}$' + f' \n using Wiener, K = {K}')
plt.show()
