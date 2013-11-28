/*********************************************************************
// created:    2007/04/12 - 16:30

//
// author:     Elie Al Alam & Gerald Dherbomez
// 
// version:    $Id: DbtPlyIOShMemTel.cpp 447 2008-05-12 11:41:12Z gdherbom $
//
// purpose:    Dbite Player In Out implementation
*********************************************************************/

#include "DbitePlayer/DbtPlyIOShMemTel.h"


DbtPlyIOShMemTel::DbtPlyIOShMemTel()
{
  size = sizeof(road_time_t) + sizeof (road_timerange_t) + sizeof (int) + 181*sizeof (float);
  createSharedMemory(size);
  openSharedMemory();
}

DbtPlyIOShMemTel::~DbtPlyIOShMemTel()
{
  closeSharedMemory();
  deleteTelemeterData(telem_data);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Bloque l'acces a la memoire partagee 
void DbtPlyIOShMemTel::lockMemory()
{
  WaitForSingleObject(sc_shmem, INFINITE); 
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Autorise l'acces a la memoire partagee
void DbtPlyIOShMemTel::unlockMemory()
{
  ReleaseSemaphore(sc_shmem, 1, NULL); 
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// fonction utilisee pour creer le memoire partagee
void DbtPlyIOShMemTel::createSharedMemory( int mapSize )
{ 
   

  // Cree le semaphore de protection de la memoire 
  sc_shmem = CreateSemaphore(NULL, 1, 1, SEM_SHMEMNAME); 
  if (sc_shmem == NULL)
  {
    printf("failed to create the semaphore protection of the shared memory - error : %d\n", GetLastError()); 
    printf("press a key to exit"); 
    getchar(); 
    exit(0); 
  }

  lockMemory(); 
  // create the file mapping 
  handle = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, mapSize, SHMEMNAME);
  if (handle == NULL) 
  {
    printf("failed to create the shared memory - error : %d\n",  GetLastError()); 
    printf("press a key to exit"); 
    getchar(); 
    exit(0); 
  }
  
  // Map the memory to a local pointer
  telem_data = (TELEMETER_DATA *) MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0); 
  if (telem_data == NULL)
  {
    printf("failed to map the view of file of the shared memory - error : %d\n", GetLastError()); 
    printf("press a key to exit"); 
    getchar(); 
    exit(0); 
  }

  
  unlockMemory(); 
  printf("La memoire partagee est creee\n"); 

}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ouvre un segment de memoire partagee existant
void DbtPlyIOShMemTel::openSharedMemory()
{
    // Cree le semaphore de protection de la memoire 
  sc_shmem = CreateSemaphore(NULL, 1, 1, SEM_SHMEMNAME); 
  if (sc_shmem == NULL)
  {
    printf("failed to create the semaphore protection of the shared memory - error : %d\n", GetLastError()); 
    printf("press a key to exit"); 
    getchar(); 
    exit(0); 
  }

  lockMemory(); 

  // create the file mapping 
  handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, SHMEMNAME);
  if (handle == NULL) 
  {
    printf("failed to open the shared memory - error : %d\n",  GetLastError()); 
    printf("press a key to exit"); 
    getchar(); 
    exit(0); 
  }

  // Map the memory to a local pointer
  telem_data = (TELEMETER_DATA	*) MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0); 
  if (telem_data == NULL)
  {
    printf("failed to map the view of file of the shared memory - error : %d\n", GetLastError()); 
    printf("press a key to exit"); 
    getchar(); 
    exit(0); 
  }

  unlockMemory(); 
  printf("La memoire partagee est ouverte\n"); 

}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// fonction de fermeture de la memoire partagee
void DbtPlyIOShMemTel::closeSharedMemory()
{
  lockMemory(); 
  UnmapViewOfFile(telem_data); 
  CloseHandle(handle); 
  unlockMemory(); 
  CloseHandle(sc_shmem); 
  
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// fontion d'ecriture en memoire partagee
void DbtPlyIOShMemTel::setData(TELEMETER_DATA data)
{
  lockMemory(); 

  // on place le poiteur sur l'adresse du segment de memoire partagee
  unsigned long * ptr = (unsigned long *)telem_data; 

  // on recopie le temps puis on deplace le pointeur 
  memcpy(ptr, &(data.time), sizeof(road_time_t) ); 
  ptr += sizeof(road_time_t); 

  // on recopie le timerange puis on deplace le pointeur 
  memcpy(ptr, &(data.tr), sizeof(road_timerange_t) ); 
  ptr += sizeof(road_timerange_t); 

  // on recopie la taille puis on deplace le pointeur 
  memcpy(ptr, &(data.size), sizeof(int) ); 
  ptr += sizeof(int); 

  // on recopie les distances 
  for (int i = 0 ; i < data.size ; ++i)
  {
    memcpy(ptr, &(data.distance[i]), sizeof(float) ); 
    ptr += sizeof(float); 
  }
 
  unlockMemory(); 
}
 

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// fonction d'initialisation de la structure TELEMETER_DATA  
TELEMETER_DATA* DbtPlyIOShMemTel::allocTelemeterData(int size)
{
  TELEMETER_DATA * data; 
  data = new TELEMETER_DATA; 
  data->size = size; 
  data->time = road_time(); 
  data->tr = 0; 
  data->distance = new float[size]; 
  
  return data; 
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Libere une structure TELEMETER_DATA and desalloue la memoire
void DbtPlyIOShMemTel::deleteTelemeterData(void * ptr)
{ 
  ptr = NULL; 
}
