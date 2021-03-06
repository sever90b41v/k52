#ifndef COUNTOBJECTIVEFUNCTIONTASK_H_
#define COUNTOBJECTIVEFUNCTIONTASK_H_

#include <string>
#include <boost/shared_ptr.hpp>

#include <k52/common/disallow_copy_and_assign.h>
#include <k52/optimization/i_objective_function.h>
#include <k52/parallel/i_task.h>

#ifdef BUILD_WITH_MPI

#include <k52/parallel/mpi/i_mpi_task.h>
#include <k52/parallel/mpi/i_mpi_task_result.h>

#endif

namespace k52
{
namespace optimization
{

class CountObjectiveFunctionTask

#ifdef BUILD_WITH_MPI
    : public k52::parallel::mpi::IMpiTask
#else
    : public k52::parallel::ITask
#endif

{
public:
    typedef boost::shared_ptr<CountObjectiveFunctionTask> shared_ptr;

    CountObjectiveFunctionTask();

    CountObjectiveFunctionTask(
        const IParameters* parameters,
        const IObjectiveFunction* function_to_optimize);

#ifdef BUILD_WITH_MPI

    virtual k52::parallel::mpi::IMpiTaskResult::shared_ptr CreateEmptyResult() const;

    virtual CountObjectiveFunctionTask* Clone() const;

    virtual void Send(boost::mpi::communicator* communicator, int target) const;

    virtual void Receive(boost::mpi::communicator* communicator, int source);

    virtual k52::parallel::mpi::IMpiTaskResult* Perform() const;

#else

    virtual k52::parallel::ITaskResult* Perform() const;

#endif

protected:
    const IObjectiveFunction* get_objective_function() const;
    const IParameters* get_parameters() const;

private:
    //TODO are functors thread-safe not to copy locally them?
    const IObjectiveFunction* const function_to_optimize_;
    const IParameters* const  parameters_;

    //Indicates if task was created at once - so const pointers to be used
    //Otherwise it is received - so shared_ptr's should be used
    bool was_created_;

    IParameters::shared_ptr received_parameters_;
    IObjectiveFunction::shared_ptr received_function_;

    DISALLOW_COPY_AND_ASSIGN(CountObjectiveFunctionTask);
};

}/* namespace optimization */
}/* namespace k52 */

#endif /* COUNTOBJECTIVEFUNCTIONTASK_H_ */
