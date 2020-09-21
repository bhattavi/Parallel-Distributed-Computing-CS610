#include 	<stdio.h>
#include	<stdarg.h>
#include	<signal.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<unistd.h>
#include	<setjmp.h>

#define WMODE 		0666
#define	IFLAG		(IPC_CREAT | IPC_EXCL)
#define on_line_CPUs		10
#define MAXPROCS		20
#define MaxSahredSegments	100
#define shared
#define L_FAILED		1
#define READ			0
#define WRITE			1
#define Trace			0

typedef struct {
  int fd_sem[2];
  int count;
  int nprocs;
  int s_intit_barrier_is_called;
} barrier_type;

typedef barrier_type *sbarrier_t;

typedef struct {
  int fd_sem[2];
  int free;
}lock_type;

typedef lock_type *slock_t;

int *key, *shmids, shmidsId = -1, *global_counter, *m_numprocs, *park;
int *child_running, *first_time, *turn, *last_sync, *m_fork_is_active; int *m_single_is_active, *m_park_procs_active;
int *m_lock_is_active;
int s_lock_mutex[2], s_wait_barrier_mutex[2];
unsigned *pids;
int m_lock_sem[2], child_sem[2], m_single_sem[2], parrent_wait_sem[2], m_sync_sem[2], m_sync_mutex[2],  park_sem[2];
int created = 0;
jmp_buf *position, *pos;


void m_kill_procs();
void clean();

void error_exit(char *s, int i)
{
  printf("\nERROR: %s - bye!\n", s);
  m_kill_procs();
  clean();
  exit(i);
}

void ClosePipe(int pd[])
{
  close(pd[READ]);
  close(pd[WRITE]);
}

void clean()
{ int i, TempKey = *key;

  if ((shmidsId != -1)&&(shmctl(shmidsId,IPC_RMID, (struct shmid_ds *) 0) < 0))
    error_exit("Shmctl faild  here- bye!", 1);

  for (i = 1; i <= TempKey; i++)
    if((shmids[i] != 0)&&(shmctl(shmids[i],IPC_RMID,(struct shmid_ds *) 0)< 0))
      error_exit("Shmctl faild there- bye!", 1);
  ClosePipe(s_lock_mutex);
  ClosePipe(s_wait_barrier_mutex);
  ClosePipe(m_lock_sem);
  ClosePipe(child_sem);
  ClosePipe(m_single_sem);
  ClosePipe(parrent_wait_sem);
  ClosePipe(m_sync_sem);
  ClosePipe(m_sync_mutex);
  ClosePipe(park_sem);
  //clean();
  exit(2);
}

void make_semaphore(int pd[], int n)
{ int i;
  char junk[2] = "M";

  if (pipe(pd) == -1)
    error_exit("pipe(pd) faild, can not create pipe", 1);
  for (i=1; i <= n; i++)
    if(write(pd[WRITE], junk, 2*sizeof(char)) == -1)
      error_exit("write() faild, check semaphore creation.", 1);
}

void p(int pd[])
{ char junk[2] = "P";

  if (Trace) printf("Enter p\n");

  if (read(pd[READ], junk, 2*sizeof(char)) == -1)
    error_exit("P() faild, check semaphore creation.", 1);
  else
    if (Trace) printf("In p, read from pipe the string = %s\n", junk);

  if (Trace) printf("Exit p, pd[READ] = %d\n", pd[READ]);
}

void v(int pd[])
{ char junk[2] = "V";

  if (Trace) printf("Enter v\n");

  if(write(pd[WRITE], junk, 2*sizeof(char)) == -1)
    error_exit("write() faild, check semaphore creation.", 1);
  else
    if (Trace) printf("In v, write to pipe the string = %s\n", junk);

  if (Trace) printf("Exit v\n");
}

char *shmalloc(unsigned size)
{ int *a;

  ++(*key);
  if((shmids[*key] = shmget(*key, size, IFLAG | WMODE)) ==  -1)
    error_exit("Shmget faild", 1);
  if((a = (int *)shmat(shmids[*key], (char *) 0, 0)) == (int *) -1)
    error_exit("Shmat faild", 1);
  return (char *)a;
}

void initialize()
{ int i;

  signal(SIGSEGV, clean);
  signal(SIGINT, clean);
  signal(SIGFPE, clean);
  signal(SIGTERM, clean);

  if((shmidsId = shmget(0, MaxSahredSegments * sizeof(unsigned),
    IFLAG | WMODE)) ==  -1)
    error_exit("Shmget1 faild", 1);
  if((shmids = (int *)shmat(shmidsId, (char *) 0, 0)) == (int *) -1)
    error_exit("Shmat faild", 1);

  for (i = 1; i < MaxSahredSegments; i++) shmids[i] = -1;

  if((shmids[1] = shmget(1, sizeof(unsigned), IFLAG | WMODE)) ==  -1)
    error_exit("Shmget2 faild", 1);
  if((key = (int *)shmat(shmids[1], (char *) 0, 0)) == (int *) -1)
    error_exit("Shmat faild", 1);

  *key = 1;
  pids = (unsigned *) shmalloc(on_line_CPUs * sizeof(unsigned));
  for (i = 0; i < on_line_CPUs; i++) pids[i] = 0;
  pids[0] = getpid();
  for (i = 1; i < on_line_CPUs; i++) pids[i] = 0;
  m_numprocs = (int *) shmalloc(sizeof(unsigned));
  *m_numprocs = on_line_CPUs/2;
  global_counter = (int *) shmalloc(sizeof(int));
  child_running = (int *) shmalloc(sizeof(int));
  *child_running = 0;
  first_time = (int *) shmalloc(sizeof(int));
  *first_time = 1;
  park = (int *) shmalloc(on_line_CPUs * sizeof(int));
  for (i = 1; i < on_line_CPUs; i++) park[i] = 0;
  turn = (int *) shmalloc(sizeof(int));
  position = (jmp_buf *) shmalloc(sizeof(jmp_buf));
  pos = (jmp_buf *) shmalloc(sizeof(jmp_buf));
  last_sync = (int *) shmalloc(sizeof(int));
  m_fork_is_active =  (int *) shmalloc(sizeof(int));
  *m_fork_is_active = 0;
  m_single_is_active =  (int *) shmalloc(sizeof(int));
  *m_single_is_active = 0;
  m_lock_is_active =  (int *) shmalloc(sizeof(int));
  *m_lock_is_active = 0;
  m_park_procs_active = (int *) shmalloc(sizeof(int));
  *m_park_procs_active = 0;
  make_semaphore(m_sync_sem,0);
  make_semaphore(m_sync_mutex, 1);
  make_semaphore(m_lock_sem, 1);
  make_semaphore(child_sem, 0);
  make_semaphore(m_single_sem, 0);
  make_semaphore(parrent_wait_sem, 0);
  make_semaphore(s_lock_mutex, 1);
  make_semaphore(s_wait_barrier_mutex, 1);
  make_semaphore(park_sem, 0);
 }

int m_set_procs(int nprocs)
{
  if (nprocs >= on_line_CPUs) {
    printf("cannot set more that %d processes\n", on_line_CPUs - 1);
    error_exit("system is terminated", 1);
  }
  if (*child_running)
    error_exit("cannot call m_set_procs while child of m_fork is running", 1);
  *m_numprocs = nprocs;
  return 0;
}

void m_kill_procs()
{ int i;

  if (Trace) printf("Enter m_kill_procs\n");
  if (*child_running)
    error_exit("can not call m_kill by a child of m_fork", 1);
  for (i = 1; i < *m_numprocs; i++)
    if (pids[i] != 0) {
      kill(pids[i], SIGKILL);
      pids[i] = 0;
    }
  *first_time = 1;
  if (Trace) printf("Exit m_kill_procs\n");
}

void m_fork(void (func)(), ...)
{ va_list ap;
  int i = 0, j;

  if (Trace) printf("Enter m_fork\n");

  if (!(*first_time)) m_kill_procs();
  *turn = *m_numprocs;
  *last_sync = *m_numprocs;
  *m_fork_is_active = 1;
  *global_counter = 0;
  *child_running = 1;
  long double pp[50];

  va_start(ap, func);
  for (i = 0; i < 50; i++)
    pp[i] = va_arg(ap, long double);
  va_end(ap);

  /* *child_spin = 1; */
  for (i = 1; i < *m_numprocs; i++)
    if (pids[0] == getpid())
      if (fork() == 0) {
        pids[i] = getpid();
        func(
          pp[0], pp[1],pp[2],pp[3],pp[4],pp[5],pp[6],pp[7],pp[8],pp[9],
          pp[10],pp[11],pp[12],pp[13],pp[14],pp[15],pp[16],pp[17],pp[18],pp[19],
          pp[20],pp[21],pp[22],pp[23],pp[24],pp[25],pp[26],pp[27],pp[28],pp[29],
          pp[30],pp[31],pp[32],pp[33],pp[34],pp[35],pp[36],pp[37],pp[38],pp[39],
          pp[40],pp[41],pp[42],pp[43],pp[44],pp[45],pp[46],pp[47],pp[48],pp[49]
        );
        v(child_sem);
        while(1)
          if(park[i]){
            p(park_sem);
            park[i] = 0;
            longjmp(*pos, 1);
          }
      }
  func(
    pp[0], pp[1],pp[2],pp[3],pp[4],pp[5],pp[6],pp[7],pp[8],pp[9],
    pp[10],pp[11],pp[12],pp[13],pp[14],pp[15],pp[16],pp[17],pp[18],pp[19],
    pp[20],pp[21],pp[22],pp[23],pp[24],pp[25],pp[26],pp[27],pp[28],pp[29],
    pp[30],pp[31],pp[32],pp[33],pp[34],pp[35],pp[36],pp[37],pp[38],pp[39],
    pp[40],pp[41],pp[42],pp[43],pp[44],pp[45],pp[46],pp[47],pp[48],pp[49]
  );
  for (j = 1; j < *m_numprocs; j++)
    p(child_sem);
  *child_running = 0;
  *m_fork_is_active = 0;
  *first_time = 0;

  if (Trace) printf("Exit m_fork\n");
}

int m_get_myid()
{ int i = 0;
  unsigned pid;

  pid = getpid();
  while ((i < *m_numprocs) && (pids[i] != pid))
    i++;
  return i;
}

void m_lock()
{
  *m_lock_is_active = 1;
  p(m_lock_sem);
}

void m_unlock()
{
  v(m_lock_sem);
  *m_lock_is_active = 0;
}

void m_sync()
{ int i;

  if (!(*m_fork_is_active))
    error_exit("must have multiple processes in execution, when m_sync is called", 1);
  if (*m_single_is_active) error_exit("must not call m_sync, when m_single is in execution", 1);
  if (*m_lock_is_active) error_exit("must not call m_sync, when m_lock is called", 1);
  p(m_sync_mutex);
  if (*last_sync != 1) {
    *last_sync = *last_sync - 1;
    v(m_sync_mutex);
    p(m_sync_sem);
    *last_sync = *last_sync + 1;
  }
  else
    for (i = 1; i < *m_numprocs; i++) v(m_sync_sem);
  if(*last_sync == *m_numprocs) v(m_sync_mutex);
}



