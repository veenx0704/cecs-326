
/**
 * FCFS scheduling algorithm.
 * First come First Serve
 */

import java.util.*;

// Your code here:
public class FCFS implements Algorithm // Implements Algorithm interface, containing schedule() and pickNextTask()
                                       // methods
{
  private List<Task> tasklist; // List of tasks
  private Task currentTask; // current task being processed

  // FCFS constructor
  public FCFS(List<Task> tasklist) {
    System.out.println("First Come First Serve\n");
    this.tasklist = tasklist;

  }

  // We are overriding the schedule() method from the Algorithm interface
  @Override
  public void schedule() {

    while (!tasklist.isEmpty()) {
      currentTask = pickNextTask();
      CPU.run(currentTask, currentTask.getBurst());

      System.out.println("Task " + currentTask.getName() + " finished\n");
    }

  }

  // We are overriding the pickNextTask() method from the Algorithm interface
  @Override
  public Task pickNextTask() {
    return tasklist.remove(0);
  }

}