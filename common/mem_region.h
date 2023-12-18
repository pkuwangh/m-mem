#ifndef __COMMON_MEM_REGION_H__
#define __COMMON_MEM_REGION_H__

#include <cstdint>
#include <memory>
#include <vector>

namespace mm_utils {

enum class MemType : int {
  NATIVE = 0,
  NODE0,
  NODE1,
  NODE2,
  NODE3,
  NODE4,
  NODE5,
  NODE6,
  NODE7,
  INTERLEAVE,
};


enum class HugePageType : int {
  NONE = 0,
  HGPG_2MB,
  HGPG_512MB,
  HGPG_1GB,
  HGPG_16GB,
};


class MemRegion {
  public:
    using Handle = std::shared_ptr<MemRegion>;

    MemRegion(
      uint64_t size,
      uint64_t active_size,
      uint64_t page_size,
      uint64_t line_size,
      MemType mem_type,
      HugePageType hugepage_type);

    MemRegion(
      uint64_t size,
      uint64_t page_size,
      uint64_t line_size) :
      MemRegion(size, size, page_size, line_size, MemType::NATIVE, HugePageType::NONE)
    { }

    MemRegion(
      uint64_t size,
      uint64_t active_size,
      uint64_t page_size,
      uint64_t line_size) :
      MemRegion(size, active_size, page_size, line_size, MemType::NATIVE, HugePageType::NONE)
    { }

    MemRegion(
      uint64_t size,
      uint64_t page_size,
      uint64_t line_size,
      MemType mem_type) :
      MemRegion(size, size, page_size, line_size, mem_type, HugePageType::NONE)
    { }

    MemRegion(
      uint64_t size,
      uint64_t page_size,
      uint64_t line_size,
      HugePageType hugepage_type) :
      MemRegion(size, size, page_size, line_size, MemType::NATIVE, hugepage_type)
    { }

    MemRegion(
      uint64_t size,
      uint64_t page_size,
      uint64_t line_size,
      MemType mem_type,
      HugePageType hugepage_type) :
      MemRegion(size, size, page_size, line_size, mem_type, hugepage_type)
    { }

    virtual ~MemRegion();

    // initialize to different patterns
    void stride_init();
    void page_random_init();
    void all_random_init();
    // helper
    void dump();
    uint64_t numAllLines() const { return num_all_pages_ * num_lines_in_page_; }
    uint64_t numActiveLines() const { return num_active_pages_ * num_lines_in_page_; }
    uint64_t activeSize() const { return active_size_; }
    uint64_t lineSize() const { return line_size_; }
    // entry point
    char** getStartPoint() const { return (char**)getOffsetAddr_(0); }
    char** getHalfPoint() const { return (char**)getOffsetAddr_(active_size_ / 2); }

  private:
    void error_(std::string message);
    char* allocNative_(const uint64_t& size);
    void randomizeSequence_(
        std::vector<uint64_t>& sequence,
        uint64_t size,
        uint64_t unit,
        bool in_order=false);
    char* getOffsetAddr_(uint64_t offset) const;

    uint64_t size_;         // size of memory region in Bytes
    uint64_t active_size_;  // active size of memory region in Bytes
    uint64_t os_page_size_; // os page size in Bytes, for alignment
    uint64_t page_size_;    // not meant to be OS page size; better to be multiple of OS page
    uint64_t line_size_;    // not necessarily the cacheline size; i.e. preferred spatial stride
    MemType mem_type_ = MemType::NATIVE;
    HugePageType hugepage_type_ = HugePageType::NONE;

    char*    addr1_ = nullptr;
    uint64_t real_size_ = 0;

    uint64_t num_all_pages_;
    uint64_t num_active_pages_;
    uint64_t num_lines_in_page_;
};

}

#endif
