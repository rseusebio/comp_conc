import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.Writer;
import java.util.Collections;
import java.util.List;
import java.util.Vector;
import java.util.stream.Stream;

/**
 * Thread consumidora
 * Responsável por ordernas as linhas
 * e inserí-las em um arquivo de saída.
 */
class Consumer extends Thread 
{
  int id;
  ProdConMonitor monitor;
  WriterMonitor wMonitor;
  String outputFileName;
  BlockCount blockCounter;

  Consumer(int id, ProdConMonitor m, WriterMonitor wm, String ofn, BlockCount b) 
  {
    this.id = id;
    monitor = m;
    wMonitor = wm;
    outputFileName = ofn;
    blockCounter = b;
  }

  public void run() 
  {
    try 
    {
      while (!blockCounter.isItDone())
      { 
        blockCounter.Decrement();

        Vector<Integer> item = monitor.Remove(id);

        Collections.sort(item);

        List<String> list = Stream.of(item.toArray()).map(String::valueOf).toList();

        String sortedRow = String.join(" ", list) + "\n";

        wMonitor.Enter(id);

        FileWriter file = new FileWriter(outputFileName, true);

        Writer writer = new BufferedWriter(file);

        writer.append(sortedRow);

        writer.close();

        wMonitor.Leave(id);
      }
    }
    catch(Exception ex) 
    {
      System.out.println("C" + id + " error: " + ex.getMessage());

      ex.printStackTrace();

      System.exit(-1);
    }
  }
}