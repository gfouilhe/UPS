import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import convolve2d
from scipy.fft import fft2, ifft2

i_ideal = plt.imread('coat_of_arms.png')

# plt.imshow(i_ideal,cmap='gray')
# plt.title('$i_{ideal}$')
# plt.show()

h = 1/(20**2)*np.ones((20,20))

i = convolve2d(i_ideal, h, mode='same', boundary='wrap')

M,N  = i.shape

# plt.imshow(i,cmap='gray')
# plt.title('$i$')
# plt.show()

I = fft2(i)
I_ideal = fft2(i_ideal)
H = fft2(h, s=i.shape)

# plt.figure(figsize =(10,30))

# plt.subplot(1,3,1)
# plt.imshow(np.log(abs(I)),cmap='gray')
# plt.title('log abs $I$')

# plt.subplot(1,3,2)
# plt.imshow(np.log(abs(I_ideal)),cmap='gray')
# plt.title('log abs $I_{ideal}$')

# plt.subplot(1,3,3)
# plt.imshow(np.log(abs(H)),cmap='gray')
# plt.title('log abs $H$')

# plt.show()

########################Division sans seuillage########################

# pseudo_inv_I = np.zeros((M,N))

# for m in range(600):
#     for n in range(800):
#         pseudo_inv_I[m,n] = I[m,n]/(H[m,n]) 

# plt.figure(figsize =(10,30))

# plt.subplot(1,2,1)
# plt.imshow(np.log(abs(pseudo_inv_I)),cmap='gray')
# plt.title('log abs reconstructed $I$ with pseudo-inverse')

# plt.subplot(1,2,2)
# plt.imshow(np.real(ifft2(pseudo_inv_I)),cmap='gray')
# plt.title('Reconstructed $i_{ideal}$ with pseudo-inverse')

# plt.show()

########################Division avec seuillage########################

pseudo_inv_I = np.zeros_like(I)

S = 0.1

for m in range(M):
    for n in range(N):
        if abs(H[m,n]) < S:
            pseudo_inv_I[m,n] = I[m,n]
        else:
            pseudo_inv_I[m,n] = I[m,n]/H[m,n] 


plt.figure(figsize = (10,30))

plt.subplot(1,2,1)
plt.imshow(np.log(abs(pseudo_inv_I)),cmap='gray')
plt.title('log abs $I_{Rec}$'+f' \n using pseudo-inverse, threshold S = {S}')

plt.subplot(1,2,2)
plt.imshow(np.real(ifft2(pseudo_inv_I)),cmap='gray')
plt.title('$i_{Rec}$' + f' \n using pseudo-inverse, threshold S = {S}')

plt.show()

########################Avec Wiener########################

weiner_I = np.zeros_like(I)

K = 10**(-5)
for m in range(M):
    for n in range(N):
        Hmn = H[m,n]
        weiner_I[m,n] = I[m,n]*np.conj(Hmn)/(abs(Hmn)**2+K)

plt.figure(figsize =(10,30))

plt.subplot(1,2,1)
plt.imshow(np.log(abs(weiner_I)),cmap='gray')
plt.title('log abs $I_{Rec}$'+f' \n using Wiener, K = {K}')

plt.subplot(1,2,2)
plt.imshow(np.real(ifft2(weiner_I)),cmap='gray')
plt.title('$i_{Rec}$' + f' \n using Wiener, K = {K}')

plt.show()

########################Avec Wiener + Bruit ########################

# weiner_I = np.zeros_like(I)

# b = np.random.normal(size=(M,N), scale=0.25)

# i = convolve2d(i_ideal,h, mode='same', boundary='wrap') + b

# K = 0.001
# for m in range(M):
#     for n in range(N):
#         Hmn = H[m,n]
#         weiner_I[m,n] = I[m,n]*np.conj(Hmn)/(abs(Hmn)**2+K)