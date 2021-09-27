import java.util.LinkedList;

class FilaTarefas 
{
    private int nThreads;
    private MyPoolThreads[] threads;
    // similar a um vetor de objetos Runable
    private LinkedList<Runnable> queue;
    private boolean shutdown;

    public FilaTarefas(int nThreads) 
    {
        this.shutdown = false;
        this.nThreads = nThreads;

        queue = new LinkedList<Runnable>();

        threads = new MyPoolThreads[nThreads];

        for (int i = 0; i < nThreads; i++) 
        {
            threads[i] = new MyPoolThreads(i);
            threads[i].start();
        }
    }

    public void execute(Runnable r) 
    {
        synchronized (queue) 
        {
            if (this.shutdown)
                return;
                
            queue.addLast(r); // inclui um novo elemento na lista ’queue’
            queue.notify();
        }
    }

    public void shutdown() 
    {
        synchronized (queue) 
        {
            this.shutdown = true;

            queue.notifyAll();
        }

        System.out.println("\nFinish them all\n".toUpperCase());

        for (int i = 0; i < nThreads; i++)
            try 
            {
                // synchronized (queue)
                // {
                //     queue.notify();
                // }

                threads[i].join();
            } 
            catch (InterruptedException e) 
            {
                return;
            }
    }

    private class MyPoolThreads extends Thread 
    {
        private int id;

        MyPoolThreads(int id)
        {
            this.id = id;
        }

        public void run() 
        {
            System.out.println("T" + this.id + " has started");

            Runnable r;
            while (true) 
            {
                synchronized (queue) 
                {
                    System.out.println("T" + this.id + " checks if queue is empty.");
                    // verifica se a lista está vazia...
                    while (queue.isEmpty() && (!shutdown)) 
                    {
                        try 
                        {
                            System.out.println("T" + this.id + " waits.");
                            queue.wait();
                            System.out.println("T" + this.id + " came back.");
                        } 
                        catch (InterruptedException ignored) 
                        {
                        }
                    }

                    if (queue.isEmpty() && shutdown)
                    {
                        System.out.println("T" + this.id + " is shutting down"); 

                        return;
                    }
                        
                    // retira o primeiro elemento da lista e o retorna
                    r = (Runnable) queue.removeFirst();
                }
                
                try 
                {
                    System.out.println("Thread " + this.id + " executed func.");

                    r.run();
                } 
                catch (RuntimeException e)
                {
                }
            }
        }
    }
}