import java.util.concurrent.ThreadLocalRandom;
  
  class MyThread extends Thread 
  {
     private int id;
     int[] vector; 
     int SIZE;
     int Threads;
     int count;
    
     public MyThread(int tid, int[] vector, int size, int t) 
     { 
        this.id = tid; 
        this.vector = vector;
        this.SIZE = size;
        this.Threads = t;

        this.count = 0;
     }
  
     public void run() 
     {
        System.out.println("Thread " + this.id + " started!");

        for (int i=id; i<this.SIZE; i += this.Threads) 
        {
           if (this.vector[i] % 2 == 0)
           {
                this.count++;
           } 
        }

        System.out.println("Thread " + this.id + " finished!"); 
     }
  }
  
  class Lab6 
  {
     public static void main (String[] args) 
     {
        if (args.length < 2)
        {
            System.out.println("java Lab6 <vector_size> <threads_number>");

            return;
        }

        int SIZE = Integer.parseInt(args[0]);
        int N = Integer.parseInt(args[1]);
        
        int[] vector = new int[SIZE];
        int evens = 0;
        for (int i = 0; i < SIZE; i++)
        {
            vector[i] = ThreadLocalRandom.current().nextInt(1, 101);

            if (vector[i] % 2 == 0)
            {
                evens++;
            }
        }

        System.out.println("Number of evens created: " + evens);

        //create threads list
        MyThread[] threads = new MyThread[N];
    
        //create threads
        for (int i=0; i<threads.length; i++) 
        {
           threads[i] = new MyThread(i, vector, SIZE, N);
        }
  
        //start the threads
        for (int i=0; i<threads.length; i++) 
        {
           threads[i].start();
        }

        int acc = 0;
  
        for (int i=0; i<threads.length; i++) 
        {
           try 
           { 
               threads[i].join(); 

               System.out.println( "thread: " + i + " found: " + threads[i].count + ".");

               acc += threads[i].count;
            } 
            catch (InterruptedException e) 
            { 
                System.out.println("Found an error");

                return; 
            }
        }

        if (acc == evens)
        {
            System.out.println("Correcet! Found: " + acc + " evens.");
        }
        else 
        {
            System.out.println("Wrong! Expected: " + evens + " but found: " + acc + ".");
        }
    }
  }
  