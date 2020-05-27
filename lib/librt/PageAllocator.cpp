/*
 * Copyright (C) 2009 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FreeNOS/System.h>
#include "PageAllocator.h"

PageAllocator::PageAllocator(const Allocator::Range range)
    : Allocator(range)
    , m_allocated(PAGESIZE)
{
}

Size PageAllocator::available() const
{
    return size() - m_allocated;
}

Allocator::Result PageAllocator::allocate(Allocator::Range & args)
{
    Memory::Range range;

    // Set return address
    args.address = base() + m_allocated;

    Size bytes  = args.size > MinimumAllocationSize ?
                  args.size : MinimumAllocationSize;

    // Align to pagesize
    bytes = aligned(bytes, PAGESIZE);

    // Fill in the message
    range.size   = bytes;
    range.access = Memory::User | Memory::Readable | Memory::Writable;
    range.virt   = base() + m_allocated;
    range.phys   = ZERO;
    const API::Result r = VMCtl(SELF, Map, &range);
    if (r != API::Success)
    {
        ERROR("failed to allocate memory using VMCtl(): " << (int)r);
        return Allocator::OutOfMemory;
    }

    // Clear the pages
    MemoryBlock::set((void *) range.virt, 0, range.size);

    // Update count
    m_allocated += range.size;

    // Success
    args.size = range.size;
    return Success;
}

Allocator::Result PageAllocator::release(const Address addr)
{
    return InvalidAddress;
}
