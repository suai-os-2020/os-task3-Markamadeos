#include "lab3.h"
#include <windows.h>
#include <iostream>

HANDLE hThread[9];
DWORD ThreadID;
HANDLE hMutex;
HANDLE semaphore_e,
semaphore_b,
semaphore_c;

DWORD WINAPI thread_b(LPVOID lpParam);
DWORD WINAPI thread_c(LPVOID lpParam);
DWORD WINAPI thread_d(LPVOID lpParam);
DWORD WINAPI thread_e(LPVOID lpParam);
DWORD WINAPI thread_f(LPVOID lpParam);
DWORD WINAPI thread_g(LPVOID lpParam);
DWORD WINAPI thread_h(LPVOID lpParam);
DWORD WINAPI thread_i(LPVOID lpParam);

unsigned int lab3_thread_graph_id() 
{
    return 8;
}

const char* lab3_unsynchronized_threads()
{
    return "deg";
}

const char* lab3_sequential_threads()
{
    return "bce";
}

//thread A
DWORD WINAPI thread_a(LPVOID lpParam) {
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'a' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	
	// start thread B
	hThread[1] = CreateThread(NULL, 0, thread_b, NULL, 0, &ThreadID); 
	if (hThread[1] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// start thread C
	hThread[2] = CreateThread(NULL, 0, thread_c, NULL, 0, &ThreadID);
	if (hThread[2] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// start thread E
	hThread[4] = CreateThread(NULL, 0, thread_e, NULL, 0, &ThreadID);
	if (hThread[4] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// wait thread B
	WaitForSingleObject(hThread[1], INFINITE);
	// close thread B
	CloseHandle(hThread[1]);
	// wait thread C
	WaitForSingleObject(hThread[2], INFINITE);
	// close thread C
	CloseHandle(hThread[2]);
	// wait thread E
	WaitForSingleObject(hThread[4], INFINITE);
	// close thread E
	CloseHandle(hThread[4]);
	return 0;
}

//thread B sem
DWORD WINAPI thread_b(LPVOID lpParam) {
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(semaphore_b, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'b' << std::flush;
		ReleaseMutex(hMutex);
		computation();
		ReleaseSemaphore(semaphore_c, 1, NULL);
	}
	return 0;
}

//thread C sem
DWORD WINAPI thread_c(LPVOID lpParam) {
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(semaphore_c, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'c' << std::flush;
		ReleaseMutex(hMutex);
		computation();
		ReleaseSemaphore(semaphore_e, 1, NULL);
	}
	return 0;
}

//thread D
DWORD WINAPI thread_d(LPVOID lpParam) {
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'd' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	return 0;
}

//thread E sem
DWORD WINAPI thread_e(LPVOID lpParam) {
	// work with semaphore
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(semaphore_e, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'e' << std::flush;
		ReleaseMutex(hMutex);
		computation();
		ReleaseSemaphore(semaphore_b, 1, NULL);
	}

	// start thread D
	hThread[3] = CreateThread(NULL, 0, thread_d, NULL, 0, &ThreadID);
	if (hThread[3] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// start thread G
	hThread[6] = CreateThread(NULL, 0, thread_g, NULL, 0, &ThreadID);
	if (hThread[6] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// work without semaphore
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'e' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}

	// wait thread D
	WaitForSingleObject(hThread[3], INFINITE);
	// close thread D
	CloseHandle(hThread[3]);
	// wait thread G
	WaitForSingleObject(hThread[6], INFINITE);
	// close thread G
	CloseHandle(hThread[6]);
	return 0;
}

//thread F
DWORD WINAPI thread_f(LPVOID lpParam) {
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'f' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	return 0;
}

//thread G
DWORD WINAPI thread_g(LPVOID lpParam) {
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'g' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}

	// start thread F
	hThread[5] = CreateThread(NULL, 0, thread_f, NULL, 0, &ThreadID);
	if (hThread[5] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// start thread H
	hThread[7] = CreateThread(NULL, 0, thread_h, NULL, 0, &ThreadID);
	if (hThread[7] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// continue work thread G 
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'g' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}

	// wait thread F
	WaitForSingleObject(hThread[5], INFINITE);
	// close thread F
	CloseHandle(hThread[5]);

	// continue work thread G 
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'g' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}

	// wait thread H
	WaitForSingleObject(hThread[7], INFINITE);
	// close thread H
	CloseHandle(hThread[7]);
	return 0;
}

//thread H
DWORD WINAPI thread_h(LPVOID lpParam) {
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'h' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}

	// start thread I
	hThread[8] = CreateThread(NULL, 0, thread_i, NULL, 0, &ThreadID);
	if (hThread[8] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// continue work thread H
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'h' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	// wait thread I
	WaitForSingleObject(hThread[8], INFINITE);
	// close thread I
	CloseHandle(hThread[8]);
	return 0;
}

//thread I
DWORD WINAPI thread_i(LPVOID lpParam) {
	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << 'i' << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	return 0;
}

int lab3_init() {
	// create mutex
	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL) {
		std::cerr << "CreateMutex error: " << GetLastError() << '\n';
		return 1;
	}

	// create semaphore B
	semaphore_b = CreateSemaphore(NULL, 1, 1, NULL);
	if (semaphore_b == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n';
		return 1;
	}

	// create semaphore C
	semaphore_c = CreateSemaphore(NULL, 0, 1, NULL);
	if (semaphore_c == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n';
		return 1;
	}

	// create semaphore E
	semaphore_e = CreateSemaphore(NULL, 0, 1, NULL);
	if (semaphore_e == NULL) {
		std::cerr << "CreateSemaphore error: " << GetLastError() << '\n';
		return 1;
	}

	// start thread A
	hThread[0] = CreateThread(NULL, 0, thread_a, NULL, 0, &ThreadID);
	if (hThread[0] == NULL) {
		std::cerr << "CreateThread error: " << GetLastError() << '\n';
		return 1;
	}

	// wait A
	WaitForSingleObject(hThread[0], INFINITE);
	// close A
	CloseHandle(hThread[0]);
	CloseHandle(hMutex);
	CloseHandle(semaphore_b);
	CloseHandle(semaphore_c);
	CloseHandle(semaphore_e);
	return 0;
}
