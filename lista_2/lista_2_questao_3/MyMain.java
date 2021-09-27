class MyMain
{
    public static int NumberOfThreads = 10;
    public static int NumberOfTasks = 100;

    public static void main(String[] args)
    {
        System.out.println("It's started.");

        FilaTarefas fila = new FilaTarefas(NumberOfThreads);

        try 
        {
            Thread.sleep(10 * 1000);
        } 
        catch (InterruptedException e) 
        {
        }
       

        for(int i = 0; i < NumberOfTasks; i++)
        {
            fila.execute(new MyRunnable());
        }

        // try 
        // {
        //     Thread.sleep(10 * 1000);
        // } 
        // catch (InterruptedException e) 
        // {
        // }

        System.out.println("Shut it all down.");

        fila.shutdown();

        System.out.println("It's over.");
    }
}