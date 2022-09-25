import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import convolve2d, wiener 
from scipy.fft import fft2, ifft2
from skimage.restoration import wiener as SKwiener

i_ideal = plt.imread('coat_of_arms.png')

plt.imshow(i_ideal,cmap='gray')
plt.title('$i_{ideal}$')
plt.show()

h = 1/(20**2)*np.ones((20,20))

i = convolve2d(i_ideal, h, mode='same', boundary='wrap')

M,N  = i.shape

plt.imshow(i,cmap='gray')
plt.title('$i$')
plt.show()

I = fft2(i)
I_ideal = fft2(i_ideal)
H = fft2(h, s=i.shape)

plt.figure(figsize =(10,30))

plt.subplot(1,3,1)
plt.imshow(np.log(abs(I)),cmap='gray')
plt.title('log abs $I$')

plt.subplot(1,3,2)
plt.imshow(np.log(abs(I_ideal)),cmap='gray')
plt.title('log abs $I_{ideal}$')

plt.subplot(1,3,3)
plt.imshow(np.log(abs(H)),cmap='gray')
plt.title('log abs $H$')

plt.show()

########################Division sans seuillage########################

pseudo_inv_I = np.zeros_like(I)

for m in range(M):
    for n in range(N):
        pseudo_inv_I[m,n] = I[m,n]/(H[m,n]) 

plt.figure(figsize =(10,30))

plt.subplot(1,2,1)
plt.imshow(np.log(abs(pseudo_inv_I)),cmap='gray')
plt.title('log abs reconstructed $I$ with pseudo-inverse')

plt.subplot(1,2,2)
plt.imshow(np.real(ifft2(pseudo_inv_I)),cmap='gray')
plt.title('Reconstructed $i_{ideal}$ with pseudo-inverse')

plt.show()

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

wiener_I = np.zeros_like(I)

K = 10**(-5)
for m in range(M):
    for n in range(N):
        Hmn = H[m,n]
        wiener_I[m,n] = I[m,n]*np.conj(Hmn)/(abs(Hmn)**2+K)

plt.figure(figsize =(10,30))

plt.subplot(1,2,1)
plt.imshow(np.log(abs(wiener_I)),cmap='gray')
plt.title('log abs $I_{Rec}$'+f' \n using Wiener, K = {K}')

plt.subplot(1,2,2)
plt.imshow(np.real(ifft2(wiener_I)),cmap='gray')
plt.title('$i_{Rec}$' + f' \n using Wiener, K = {K}')

plt.show()

########################Avec Wiener de scipy et scikit-image ########################

plt.figure(figsize =(10,30))

plt.subplot(1,3,1)
plt.imshow(wiener(i),cmap='gray')
plt.title('$i_{Rec}$'+f' \n scipy')

plt.subplot(1,3,2)
plt.imshow(SKwiener(i,h,balance=K),cmap='gray')
plt.title('$i_{Rec}$'+f' \n scikit-image, balance = {K}')

plt.subplot(1,3,3)
plt.imshow(np.real(ifft2(wiener_I)),cmap='gray')
plt.title('$i_{Rec}$' + f' \n our implementation, K = {K}')

plt.show()

########################Avec Wiener + Bruit ########################

#Génération d'un bruit de SNR 40
SNR = 40 
P_i = sum(sum(i**2))
P_b = P_i/(10**(SNR/10))
b_0 = np.random.normal(size=(M,N))
P_b0 = sum(sum(b_0**2))
b = b_0/np.sqrt(P_b0)*np.sqrt(P_b)

i += b
I = fft2(i)

plt.imshow(i,cmap='gray')
plt.title('$i_{bruitée}$')
plt.show()

wiener_I = np.zeros_like(I)

K = 0.001
for m in range(M):
    for n in range(N):
        Hmn = H[m,n]
        wiener_I[m,n] = I[m,n]*np.conj(Hmn)/(abs(Hmn)**2+K)

plt.figure(figsize =(10,30))

plt.subplot(1,2,1)
plt.imshow(np.log(abs(wiener_I)),cmap='gray')
plt.title('log abs $I_{bruitée}$'+f' \n using Wiener, K = {K}')

plt.subplot(1,2,2)
plt.imshow(np.real(ifft2(wiener_I)),cmap='gray')
plt.title('$i_{bruitée}$' + f' \n using Wiener, K = {K}')

plt.show()

plt.figure(figsize =(10,30))

plt.subplot(1,3,1)
plt.imshow(wiener(i),cmap='gray')
plt.title('$i_{Rec}$'+f' \n scipy')

plt.subplot(1,3,2)
plt.imshow(SKwiener(i,h,balance=K),cmap='gray')
plt.title('$i_{Rec}$'+f' \n scikit-image, balance = {K}')

plt.subplot(1,3,3)
plt.imshow(np.real(ifft2(wiener_I)),cmap='gray')
plt.title('$i_{Rec}$' + f' \n our implementation, K = {K}')

plt.show()


########################Recherche Linéaire ########################

plt.figure(figsize =(50,10))

Klist = [0.0001,0.0005,0.001,0.005,0.01]
for i,K in enumerate(Klist):
    wiener_I = np.zeros_like(I)
    for m in range(M):
        for n in range(N):
            Hmn = H[m,n]
            wiener_I[m,n] = I[m,n]*np.conj(Hmn)/(abs(Hmn)**2+K)

    plt.subplot(5,1,i+1)
    plt.imshow(np.real(ifft2(wiener_I)),cmap='gray')
    plt.title(f'K = {K}')

plt.show()