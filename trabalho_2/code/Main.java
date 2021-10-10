import java.io.File;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

/* Disciplina: Computacao Concorrente */
/* Aluno.: Rafael Santos Eusébio */
/* DRE: 117208725 */
/* -------------------------------------------------------------------*/
class Main 
{
  static int C;

  static String expectedArgs = "<number_of_C_threads> <block_size> <source_file_name> <dest_file_name>";

  public static boolean ValidateOutput(String outputFile, int expectedLines)
  {
    int linesCount = 0, firstElement = 0, indexOfEl =0;

    try 
    {
      File sourceFile = new File(outputFile);

      Scanner scanner = new Scanner(sourceFile);

      List<Integer> range = IntStream.rangeClosed(1, expectedLines).boxed().collect(Collectors.toList());

      while(scanner.hasNextLine())
      {
        linesCount++;

        List<Integer> fetchedList = Stream.of(scanner.nextLine().split(" ")).map(Integer::valueOf).toList();

        List<Integer> orderedList = new ArrayList<>(fetchedList);

        Collections.sort(orderedList);

        if (!orderedList.equals(fetchedList))
        {
          System.out.println("Line " + linesCount + " is not sorted correctly.\n");

          scanner.close();

          return false;
        }

        firstElement = fetchedList.get(0);
        indexOfEl = range.indexOf(firstElement);
        range.remove(indexOfEl);
      }

      scanner.close();

      if (linesCount != expectedLines)
      {
        System.out.println("Lines counted: " + linesCount + ", lines expected: " + expectedLines + "\n.");

        return false;
      }

      if (range.size() > 0)
      {
        System.out.println("Some lines are missing.\n");

        return false;
      }

      return true;
    }
    catch (Exception e)
    {
      System.out.print("Element : " + firstElement + " at index: " + indexOfEl + "\n");

      System.out.print("Found and exception: " + e.getMessage() + "\n");

      e.printStackTrace();

      return false;
    }
  }

  public static void main (String[] args) 
  {
    if (args.length < 4)
    {
      System.out.println("Arguments should be: " + expectedArgs);

      System.exit(-1);
    }

    int numberOfCThreads=0, blockSize=0, rows=0;
    String sourceFileName="", destFileName="";
    Instant start = null, end = null;

    try
    {
      numberOfCThreads = Integer.parseInt(args[0]);

      blockSize = Integer.parseInt(args[1]);

      sourceFileName = args[2];

      destFileName = args[3];
    }
    catch (Exception err)
    {
      System.out.println("Invalid Args: " + err.getMessage());

      System.exit(-1);
    }

    try 
    {
      ProdConMonitor monitor = new ProdConMonitor(blockSize);
      WriterMonitor wMonitor = new WriterMonitor();
  
      File sourceFile = new File(sourceFileName);
      Scanner scanner = new Scanner(sourceFile);
  
      rows = Integer.parseInt(scanner.nextLine());
      BlockCount blockCounter = new BlockCount(rows);
      
      Consumer[] cons = new Consumer[numberOfCThreads];

      Producer prod = new Producer(1, monitor, scanner, rows);

      start = Instant.now();

      prod.start();
      
      for (int i = 0; i < numberOfCThreads; i++)
      {
        cons[i] = new Consumer(i+1, monitor, wMonitor, destFileName, blockCounter);
  
        cons[i].start(); 
      }
  
      prod.join();
  
      for (int i = 0; i < numberOfCThreads; i++)    
      {
        cons[i].join(); 
      }

      end = Instant.now();

      scanner.close();
    }
    catch(Exception e)
    {
      System.out.println("An exception was raised: " + e.getMessage());

      System.exit(-1);
    }

    if (!ValidateOutput(destFileName, rows))
    {
      System.out.println("Wrong");

      System.exit(-1);
    }

    Duration executionTime = Duration.between(start, end);    

    System.out.println(numberOfCThreads + "," + executionTime.toSeconds() + "\n");
  }
}
