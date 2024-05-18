
/**
 * Non-preemptive priority scheduling algorithm using RR.
 *
 * This algorithm will run tasks according to round-robin scheduling.
 */

import java.util.*;

// Your code here:

//RR class for Algorithm.java
public class RR implements Algorithm {

//private list(taskHolder) and task(currentTask) and int(timeQuantumLength)
  private List<Task> taskHolder;
  private Task currentTask;
  private int timeQuantumLength = 10;

  //RR constructor
  public RR(List<Task> taskHolder) {

    //printing title
    System.out.println("Round Robin Scheduler");
    System.out.println();

    //this taskHolder
    this.taskHolder = taskHolder;
  }

  //Overriding schedule()
  @Override
  public void schedule() {

    //while taskHolder is not empty
    while (taskHolder.isEmpty() == false) {

      //currentTask is set to the first task in the taskHolder
      //runs the currentTask for the timeQuantumLength
      //set burst time to current time - timeQuantumLength
      currentTask = pickNextTask();
      CPU.run(currentTask, timeQuantumLength);
        currentTask.setBurst(currentTask.getBurst() - timeQuantumLength);

      //if the burst time is greater than or equal to 1
      // it will add the currentTask to the taskHolder
      if (currentTask.getBurst() >= 1) {
          taskHolder.add(currentTask);
      }

      //printing task name 'has completed'
      System.out.println("\nTask " + currentTask.getName() + " has completed. \n");
      System.out.println();
    }

  }

  //Override pickNextTask()
  @Override
  public Task pickNextTask() {

    //return remove the first task in the taskHolder
    return taskHolder.remove(0);
  }
}