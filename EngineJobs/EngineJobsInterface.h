//----------------------------------------------------------------------------------------------------------------------
// EngineJobsInterface.h
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

// !!! AUTOGENERATED FILE - DO NOT EDIT !!!

#pragma once
#ifndef LUNAR_ENGINE_JOBS_ENGINE_JOBS_INTERFACE_H
#define LUNAR_ENGINE_JOBS_ENGINE_JOBS_INTERFACE_H

#include "EngineJobs/EngineJobs.h"
#include "Platform/Assert.h"
#include "Foundation/Functions.h"
#include "EngineJobs/EngineJobsTypes.h"

namespace Lunar
{
    class JobContext;
}

namespace Lunar
{

/// Parallel array quick sort.
template< typename T, typename CompareFunction = Less< T > >
class SortJob : Lunar::NonCopyable
{
public:
    class Parameters
    {
    public:
        /// [inout] Pointer to the first element to sort.
        T* pBase;
        /// [in] Number of elements to sort.
        size_t count;
        /// [in] Function object for checking whether the first element should be sorted before the second element.
        CompareFunction compare;
        /// [in] Sub-division size at which to run the remainder of the sort within a single job.
        size_t singleJobCount;

        /// @name Construction/Destruction
        //@{
        inline Parameters();
        //@}
    };

    /// @name Construction/Destruction
    //@{
    inline SortJob();
    inline ~SortJob();
    //@}

    /// @name Parameters
    //@{
    inline Parameters& GetParameters();
    inline const Parameters& GetParameters() const;
    inline void SetParameters( const Parameters& rParameters );
    //@}

    /// @name Job Execution
    //@{
    void Run( JobContext* pContext );
    inline static void RunCallback( void* pJob, JobContext* pContext );
    //@}

private:
    Parameters m_parameters;
};

}  // namespace Lunar

#include "EngineJobs/EngineJobsInterface.inl"
#include "EngineJobs/SortJob.inl"

#endif  // LUNAR_ENGINE_JOBS_ENGINE_JOBS_INTERFACE_H
