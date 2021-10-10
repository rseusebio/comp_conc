import java.util.Arrays;
import java.util.Vector;
import java.util.concurrent.Semaphore;

/**
 * Monitor reponsável por sincronizar o trabalho 
 * da thread Produtora e das threads Consumidoras
 * Possui também o buffer onde será armazenado as linhas
 * a serem ordenadas
 */
class ProdConMonitor 
{
   private Vector<Vector<Integer>> buffer;
   private int bufferSize = 10, in = 0, out = 0;
   private Semaphore inSem, outSem, mutex;

  ProdConMonitor(int blockSize) 
  { 
    buffer = new Vector<Vector<Integer>>();

    for (int i = 0; i < bufferSize; i++)    
    {
      Integer[] arr = new Integer[blockSize];
      Arrays.fill(arr, 0);

      buffer.add(new Vector<Integer>(Arrays.asList(arr)));
    }

    inSem = new Semaphore(bufferSize);
    outSem = new Semaphore(0);
    mutex = new Semaphore(1);
  }

   public void Insert(int id, Vector<Integer> item) 
   {
      try 
      {
         inSem.acquire();

         mutex.acquire();

         buffer.set(in, item);

         in = (in + 1) % bufferSize;

         mutex.release();

         outSem.release();
      } 
      catch (InterruptedException err) 
      {
         System.out.println("P" + id + " raised an exception: " + err.getMessage());

         err.printStackTrace();

         System.exit(-1);
      }
   }

   public Vector<Integer> Remove(int id) 
   {
      Vector<Integer> item = new Vector<>();

      try 
      {
         outSem.acquire();

         mutex.acquire();

         item = buffer.get(out);
         
         Integer[] arr = new Integer[item.size()];

         Arrays.fill(arr, 0);

         buffer.set(out, new Vector<Integer>(Arrays.asList(arr)));

         out = (out + 1) % bufferSize;

         mutex.release();
         
         inSem.release();
      }
      catch(Exception err) 
      {
         System.out.println("C" + id + " raised an exception: " + err.getMessage());

         err.printStackTrace();

         System.exit(-1);
      }

      return item;
   }
}
