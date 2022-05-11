from ctypes import *
import numpy as np

libc = CDLL("./Dll1.dll")

#Задаем првильные форматы функциям
libc.create_simple_object.restype = c_void_p

libc.obj_print_self.argtypes = [c_void_p]

libc.create_Moving.restype = c_void_p
libc.create_Moving.argtypes = [c_double, c_double, c_double, c_char_p]

libc.transform.restype = c_void_p
libc.transform.argtypes = [c_void_p, c_void_p]


Array_simple_type = c_double*4

libc.simple_object_values.restype = POINTER(Array_simple_type)
libc.simple_object_values.argtypes = [c_void_p]

libc.delete_simple_object.restype = c_void_p
libc.delete_simple_object.argtypes = [c_void_p]

libc.delete_transformation.restype = c_void_p
libc.delete_transformation.argtypes = [c_void_p]
#Начинаем делать что-то значащее

a = libc.create_simple_object(1,2,3,4) #точка [0.25, 0.5, 0.75]

mov_id = c_char_p('tiger'.encode('utf-8'))
mov = libc.create_Moving(1,1,1, mov_id) #сдвиг на [1, 1, 1]

b = libc.transform(a,mov) #результат
print(np.frombuffer(libc.simple_object_values(b).contents))

libc.delete_simple_object(a)
libc.delete_simple_object(b)
libc.delete_transformation(mov)

#теперь выдаст ошибку:
#print(np.frombuffer(libc.simple_object_values(b).contents))
