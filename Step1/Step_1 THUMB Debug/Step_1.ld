MEMORY
{
  UNPLACED_SECTIONS (wx) : ORIGIN = 0x100000000, LENGTH = 0
  SCRATCH_Y (wx) : ORIGIN = 0x20041000, LENGTH = 0x00001000
  SCRATCH_X (wx) : ORIGIN = 0x20040000, LENGTH = 0x00001000
  RAM (wx) : ORIGIN = 0x20000000, LENGTH = 0x00040000
  FLASH (rx) : ORIGIN = 0x10000000, LENGTH = 0x01000000
  ROM (rx) : ORIGIN = 0x00000000, LENGTH = 0x00004000
}

SECTIONS
{
  __SCRATCH_Y_segment_start__ = 0x20041000;
  __SCRATCH_Y_segment_end__ = 0x20042000;
  __SCRATCH_Y_segment_size__ = 0x00001000;
  __SCRATCH_X_segment_start__ = 0x20040000;
  __SCRATCH_X_segment_end__ = 0x20041000;
  __SCRATCH_X_segment_size__ = 0x00001000;
  __RAM_segment_start__ = 0x20000000;
  __RAM_segment_end__ = 0x20040000;
  __RAM_segment_size__ = 0x00040000;
  __FLASH_segment_start__ = 0x10000000;
  __FLASH_segment_end__ = 0x11000000;
  __FLASH_segment_size__ = 0x01000000;
  __ROM_segment_start__ = 0x00000000;
  __ROM_segment_end__ = 0x00004000;
  __ROM_segment_size__ = 0x00004000;

  __HEAPSIZE__ = 1024;
  __STACKSIZE_ABT__ = 0;
  __STACKSIZE_FIQ__ = 0;
  __STACKSIZE_IRQ__ = 0;
  __STACKSIZE_PROCESS__ = 0;
  __STACKSIZE_SVC__ = 0;
  __STACKSIZE_UND__ = 0;
  __STACKSIZE__ = 1024;
  __do_debug_operation = __do_debug_operation_mempoll;
  __vfprintf = __vfprintf_float_long_long;
  __vfscanf = __vfscanf_float_long_long_cc;

  __boot_load_start__ = ALIGN(__FLASH_segment_start__ , 4);
  .boot ALIGN(__FLASH_segment_start__ , 4) : AT(ALIGN(__FLASH_segment_start__ , 4))
  {
    __boot_start__ = .;
    *(.boot .boot.*)
    . = ALIGN(MAX(__boot_start__ + 0x100 , .), 4);
  }
  __boot_end__ = __boot_start__ + SIZEOF(.boot);
  __boot_size__ = SIZEOF(.boot);
  __boot_load_end__ = __boot_end__;

  . = ASSERT(__boot_start__ == __boot_end__ || (__boot_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .boot is too large to fit in FLASH memory segment");

  . = ASSERT(0x100  == ALIGN(0x100, 4), "error: .boot section size does not match alignment");

  . = ASSERT(__boot_size__ <= 0x100 , "error: .boot section is larger than specified size");

  __vectors_load_start__ = ALIGN(__boot_end__ , 256);
  .vectors ALIGN(__boot_end__ , 256) : AT(ALIGN(__boot_end__ , 256))
  {
    __vectors_start__ = .;
    *(.vectors .vectors.*)
  }
  __vectors_end__ = __vectors_start__ + SIZEOF(.vectors);
  __vectors_size__ = SIZEOF(.vectors);
  __vectors_load_end__ = __vectors_end__;

  . = ASSERT(__vectors_start__ == __vectors_end__ || (__vectors_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .vectors is too large to fit in FLASH memory segment");

  __init_load_start__ = ALIGN(__vectors_end__ , 4);
  .init ALIGN(__vectors_end__ , 4) : AT(ALIGN(__vectors_end__ , 4))
  {
    __init_start__ = .;
    *(.init .init.*)
  }
  __init_end__ = __init_start__ + SIZEOF(.init);
  __init_size__ = SIZEOF(.init);
  __init_load_end__ = __init_end__;

  . = ASSERT(__init_start__ == __init_end__ || (__init_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .init is too large to fit in FLASH memory segment");

  __init_rodata_load_start__ = ALIGN(__init_end__ , 4);
  .init_rodata ALIGN(__init_end__ , 4) : AT(ALIGN(__init_end__ , 4))
  {
    __init_rodata_start__ = .;
    *(.init_rodata .init_rodata.*)
  }
  __init_rodata_end__ = __init_rodata_start__ + SIZEOF(.init_rodata);
  __init_rodata_size__ = SIZEOF(.init_rodata);
  __init_rodata_load_end__ = __init_rodata_end__;

  . = ASSERT(__init_rodata_start__ == __init_rodata_end__ || (__init_rodata_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .init_rodata is too large to fit in FLASH memory segment");

  __reset_load_start__ = ALIGN(__init_rodata_end__ , 4);
  .reset ALIGN(__init_rodata_end__ , 4) : AT(ALIGN(__init_rodata_end__ , 4))
  {
    __reset_start__ = .;
    *(.reset .reset.*)
  }
  __reset_end__ = __reset_start__ + SIZEOF(.reset);
  __reset_size__ = SIZEOF(.reset);
  __reset_load_end__ = __reset_end__;

  . = ASSERT(__reset_start__ == __reset_end__ || (__reset_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .reset is too large to fit in FLASH memory segment");

  __text_load_start__ = ALIGN(__reset_end__ , 4);
  .text ALIGN(__reset_end__ , 4) : AT(ALIGN(__reset_end__ , 4))
  {
    __text_start__ = .;
    *(.text .text.* .gnu.linkonce.t.* .glue_7t .glue_7 .gcc_except_table .ARM.extab* .gnu.linkonce.armextab.*)
  }
  __text_end__ = __text_start__ + SIZEOF(.text);
  __text_size__ = SIZEOF(.text);
  __text_load_end__ = __text_end__;

  . = ASSERT(__text_start__ == __text_end__ || (__text_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .text is too large to fit in FLASH memory segment");

  __dtors_load_start__ = ALIGN(__text_end__ , 4);
  .dtors ALIGN(__text_end__ , 4) : AT(ALIGN(__text_end__ , 4))
  {
    __dtors_start__ = .;
    KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors)) KEEP (*(.fini_array)) KEEP (*(SORT(.fini_array.*)))
  }
  __dtors_end__ = __dtors_start__ + SIZEOF(.dtors);
  __dtors_size__ = SIZEOF(.dtors);
  __dtors_load_end__ = __dtors_end__;

  . = ASSERT(__dtors_start__ == __dtors_end__ || (__dtors_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .dtors is too large to fit in FLASH memory segment");

  __ctors_load_start__ = ALIGN(__dtors_end__ , 4);
  .ctors ALIGN(__dtors_end__ , 4) : AT(ALIGN(__dtors_end__ , 4))
  {
    __ctors_start__ = .;
    KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors)) KEEP (*(.init_array)) KEEP (*(SORT(.init_array.*)))
  }
  __ctors_end__ = __ctors_start__ + SIZEOF(.ctors);
  __ctors_size__ = SIZEOF(.ctors);
  __ctors_load_end__ = __ctors_end__;

  . = ASSERT(__ctors_start__ == __ctors_end__ || (__ctors_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .ctors is too large to fit in FLASH memory segment");

  __rodata_load_start__ = ALIGN(__ctors_end__ , 4);
  .rodata ALIGN(__ctors_end__ , 4) : AT(ALIGN(__ctors_end__ , 4))
  {
    __rodata_start__ = .;
    *(.rodata .rodata.* .gnu.linkonce.r.* .data.rel.ro .data.rel.ro.*)
  }
  __rodata_end__ = __rodata_start__ + SIZEOF(.rodata);
  __rodata_size__ = SIZEOF(.rodata);
  __rodata_load_end__ = __rodata_end__;

  . = ASSERT(__rodata_start__ == __rodata_end__ || (__rodata_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .rodata is too large to fit in FLASH memory segment");

  __ARM.exidx_load_start__ = ALIGN(__rodata_end__ , 4);
  .ARM.exidx ALIGN(__rodata_end__ , 4) : AT(ALIGN(__rodata_end__ , 4))
  {
    __ARM.exidx_start__ = .;
    __exidx_start =   __ARM.exidx_start__;
    *(.ARM.exidx .ARM.exidx.*)
  }
  __ARM.exidx_end__ = __ARM.exidx_start__ + SIZEOF(.ARM.exidx);
  __ARM.exidx_size__ = SIZEOF(.ARM.exidx);
  __exidx_end =   __ARM.exidx_end__;
  __ARM.exidx_load_end__ = __ARM.exidx_end__;

  . = ASSERT(__ARM.exidx_start__ == __ARM.exidx_end__ || (__ARM.exidx_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .ARM.exidx is too large to fit in FLASH memory segment");

  __ram_vector_table_load_start__ = ALIGN(__RAM_segment_start__ , 256);
  .ram_vector_table ALIGN(__RAM_segment_start__ , 256) (NOLOAD) : AT(ALIGN(__RAM_segment_start__ , 256))
  {
    __ram_vector_table_start__ = .;
    *(.ram_vector_table .ram_vector_table.*)
  }
  __ram_vector_table_end__ = __ram_vector_table_start__ + SIZEOF(.ram_vector_table);
  __ram_vector_table_size__ = SIZEOF(.ram_vector_table);
  __ram_vector_table_load_end__ = __ram_vector_table_end__;

  . = ASSERT(__ram_vector_table_start__ == __ram_vector_table_end__ || (__ram_vector_table_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .ram_vector_table is too large to fit in RAM memory segment");

  __fast_load_start__ = ALIGN(__ARM.exidx_end__ , 4);
  .fast ALIGN(__ram_vector_table_end__ , 4) : AT(ALIGN(__ARM.exidx_end__ , 4))
  {
    __fast_start__ = .;
    *(.fast .fast.* .time_critical.*)
  }
  __fast_end__ = __fast_start__ + SIZEOF(.fast);
  __fast_size__ = SIZEOF(.fast);
  __fast_load_end__ = __fast_load_start__ + SIZEOF(.fast);

  . = ASSERT(__fast_load_start__ == __fast_load_end__ || (__fast_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .fast is too large to fit in FLASH memory segment");

  .fast_run ALIGN(__ram_vector_table_end__ , 4) (NOLOAD) :
  {
    __fast_run_start__ = .;
  }
  __fast_run_end__ = __fast_run_start__ + SIZEOF(.fast);
  __fast_run_size__ = __fast_run_end__ - __fast_run_start__;
  __fast_run_load_end__ = __fast_run_end__;

  . = ASSERT(__fast_run_start__ == __fast_run_end__ || (__fast_run_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .fast_run is too large to fit in RAM memory segment");

  __bss_load_start__ = ALIGN(__fast_run_end__ , 4);
  .bss ALIGN(__fast_run_end__ , 4) (NOLOAD) : AT(ALIGN(__fast_run_end__ , 4))
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __bss_end__ = __bss_start__ + SIZEOF(.bss);
  __bss_size__ = SIZEOF(.bss);
  __bss_load_end__ = __bss_end__;

  . = ASSERT(__bss_start__ == __bss_end__ || (__bss_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .bss is too large to fit in RAM memory segment");

  __tbss_load_start__ = ALIGN(__bss_end__ , 4);
  .tbss ALIGN(__bss_end__ , 4) (NOLOAD) : AT(ALIGN(__bss_end__ , 4))
  {
    __tbss_start__ = .;
    *(.tbss .tbss.*)
  }
  __tbss_end__ = __tbss_start__ + SIZEOF(.tbss);
  __tbss_size__ = SIZEOF(.tbss);
  __tbss_load_end__ = __tbss_end__;

  . = ASSERT(__tbss_start__ == __tbss_end__ || (__tbss_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .tbss is too large to fit in RAM memory segment");

  __non_init_load_start__ = ALIGN(__tbss_end__ , 4);
  .non_init ALIGN(__tbss_end__ , 4) (NOLOAD) : AT(ALIGN(__tbss_end__ , 4))
  {
    __non_init_start__ = .;
    *(.non_init .non_init.* .uninitialized_data.*)
  }
  __non_init_end__ = __non_init_start__ + SIZEOF(.non_init);
  __non_init_size__ = SIZEOF(.non_init);
  __non_init_load_end__ = __non_init_end__;

  . = ASSERT(__non_init_start__ == __non_init_end__ || (__non_init_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .non_init is too large to fit in RAM memory segment");

  __tdata_load_start__ = ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4);
  .tdata ALIGN(__non_init_end__ , 4) : AT(ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4))
  {
    __tdata_start__ = .;
    *(.tdata .tdata.*)
  }
  __tdata_end__ = __tdata_start__ + SIZEOF(.tdata);
  __tdata_size__ = SIZEOF(.tdata);
  __tdata_load_end__ = __tdata_load_start__ + SIZEOF(.tdata);

  . = ASSERT(__tdata_load_start__ == __tdata_load_end__ || (__tdata_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .tdata is too large to fit in FLASH memory segment");

  .tdata_run ALIGN(__non_init_end__ , 4) (NOLOAD) :
  {
    __tdata_run_start__ = .;
  }
  __tdata_run_end__ = __tdata_run_start__ + SIZEOF(.tdata);
  __tdata_run_size__ = __tdata_run_end__ - __tdata_run_start__;
  __tdata_run_load_end__ = __tdata_run_end__;

  . = ASSERT(__tdata_run_start__ == __tdata_run_end__ || (__tdata_run_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .tdata_run is too large to fit in RAM memory segment");

  __data_load_start__ = ALIGN(__tdata_load_start__ + SIZEOF(.tdata) , 4);
  .data ALIGN(__tdata_run_end__ , 4) : AT(ALIGN(__tdata_load_start__ + SIZEOF(.tdata) , 4))
  {
    __data_start__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
  }
  __data_end__ = __data_start__ + SIZEOF(.data);
  __data_size__ = SIZEOF(.data);
  __data_load_end__ = __data_load_start__ + SIZEOF(.data);

  . = ASSERT(__data_load_start__ == __data_load_end__ || (__data_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .data is too large to fit in FLASH memory segment");

  .data_run ALIGN(__tdata_run_end__ , 4) (NOLOAD) :
  {
    __data_run_start__ = .;
  }
  __data_run_end__ = __data_run_start__ + SIZEOF(.data);
  __data_run_size__ = __data_run_end__ - __data_run_start__;
  __data_run_load_end__ = __data_run_end__;

  . = ASSERT(__data_run_start__ == __data_run_end__ || (__data_run_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .data_run is too large to fit in RAM memory segment");

  __mutex_load_start__ = ALIGN(__data_load_start__ + SIZEOF(.data) , 4);
  .mutex ALIGN(__data_run_end__ , 4) : AT(ALIGN(__data_load_start__ + SIZEOF(.data) , 4))
  {
    __mutex_start__ = .;
    *(.mutex_array .mutex_array.* )
  }
  __mutex_end__ = __mutex_start__ + SIZEOF(.mutex);
  __mutex_size__ = SIZEOF(.mutex);
  __mutex_load_end__ = __mutex_load_start__ + SIZEOF(.mutex);

  . = ASSERT(__mutex_load_start__ == __mutex_load_end__ || (__mutex_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .mutex is too large to fit in FLASH memory segment");

  .mutex_run ALIGN(__data_run_end__ , 4) (NOLOAD) :
  {
    __mutex_run_start__ = .;
    __mutex_array_start =   __mutex_run_start__;
  }
  __mutex_run_end__ = __mutex_run_start__ + SIZEOF(.mutex);
  __mutex_run_size__ = __mutex_run_end__ - __mutex_run_start__;
  __mutex_array_end =   __mutex_run_end__;
  __mutex_run_load_end__ = __mutex_run_end__;

  . = ASSERT(__mutex_run_start__ == __mutex_run_end__ || (__mutex_run_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .mutex_run is too large to fit in RAM memory segment");

  __after_data_load_start__ = ALIGN(__mutex_load_start__ + SIZEOF(.mutex) , 4);
  .after_data ALIGN(__mutex_run_end__ , 4) : AT(ALIGN(__mutex_load_start__ + SIZEOF(.mutex) , 4))
  {
    __after_data_start__ = .;
    *(.after_data .after_data.*)
  }
  __after_data_end__ = __after_data_start__ + SIZEOF(.after_data);
  __after_data_size__ = SIZEOF(.after_data);
  __after_data_load_end__ = __after_data_load_start__ + SIZEOF(.after_data);

  . = ASSERT(__after_data_load_start__ == __after_data_load_end__ || (__after_data_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .after_data is too large to fit in FLASH memory segment");

  __scratch_x_load_start__ = ALIGN(__after_data_load_start__ + SIZEOF(.after_data) , 4);
  .scratch_x ALIGN(__SCRATCH_X_segment_start__ , 4) : AT(ALIGN(__after_data_load_start__ + SIZEOF(.after_data) , 4))
  {
    __scratch_x_start__ = .;
    *(.scratch_x .scratch_x.*)
  }
  __scratch_x_end__ = __scratch_x_start__ + SIZEOF(.scratch_x);
  __scratch_x_size__ = SIZEOF(.scratch_x);
  __scratch_x_load_end__ = __scratch_x_load_start__ + SIZEOF(.scratch_x);

  . = ASSERT(__scratch_x_load_start__ == __scratch_x_load_end__ || (__scratch_x_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .scratch_x is too large to fit in FLASH memory segment");

  __scratch_y_load_start__ = ALIGN(__scratch_x_load_start__ + SIZEOF(.scratch_x) , 4);
  .scratch_y ALIGN(__SCRATCH_Y_segment_start__ , 4) : AT(ALIGN(__scratch_x_load_start__ + SIZEOF(.scratch_x) , 4))
  {
    __scratch_y_start__ = .;
    *(.scratch_y .scratch_y.*)
  }
  __scratch_y_end__ = __scratch_y_start__ + SIZEOF(.scratch_y);
  __scratch_y_size__ = SIZEOF(.scratch_y);
  __scratch_y_load_end__ = __scratch_y_load_start__ + SIZEOF(.scratch_y);

  . = ASSERT(__scratch_y_load_start__ == __scratch_y_load_end__ || (__scratch_y_load_end__ - __FLASH_segment_start__) <=  __FLASH_segment_size__ , "error: .scratch_y is too large to fit in FLASH memory segment");

  .scratch_y_run ALIGN(__SCRATCH_Y_segment_start__ , 4) (NOLOAD) :
  {
    __scratch_y_run_start__ = .;
  }
  __scratch_y_run_end__ = __scratch_y_run_start__ + SIZEOF(.scratch_y);
  __scratch_y_run_size__ = __scratch_y_run_end__ - __scratch_y_run_start__;
  __scratch_y_run_load_end__ = __scratch_y_run_end__;

  __SCRATCH_Y_segment_used_end__ = ALIGN(__SCRATCH_Y_segment_start__ , 4) + SIZEOF(.scratch_y_run);
  __SCRATCH_Y_segment_used_size__ = __SCRATCH_Y_segment_used_end__ - __SCRATCH_Y_segment_start__;

  . = ASSERT(__scratch_y_run_start__ == __scratch_y_run_end__ || (__scratch_y_run_end__ - __SCRATCH_Y_segment_start__) <= __SCRATCH_Y_segment_size__ , "error: .scratch_y_run is too large to fit in SCRATCH_Y memory segment");

  .scratch_x_run ALIGN(__SCRATCH_X_segment_start__ , 4) (NOLOAD) :
  {
    __scratch_x_run_start__ = .;
  }
  __scratch_x_run_end__ = __scratch_x_run_start__ + SIZEOF(.scratch_x);
  __scratch_x_run_size__ = __scratch_x_run_end__ - __scratch_x_run_start__;
  __scratch_x_run_load_end__ = __scratch_x_run_end__;

  __SCRATCH_X_segment_used_end__ = ALIGN(__SCRATCH_X_segment_start__ , 4) + SIZEOF(.scratch_x_run);
  __SCRATCH_X_segment_used_size__ = __SCRATCH_X_segment_used_end__ - __SCRATCH_X_segment_start__;

  . = ASSERT(__scratch_x_run_start__ == __scratch_x_run_end__ || (__scratch_x_run_end__ - __SCRATCH_X_segment_start__) <= __SCRATCH_X_segment_size__ , "error: .scratch_x_run is too large to fit in SCRATCH_X memory segment");

  .after_data_run ALIGN(__mutex_run_end__ , 4) (NOLOAD) :
  {
    __after_data_run_start__ = .;
  }
  __after_data_run_end__ = __after_data_run_start__ + SIZEOF(.after_data);
  __after_data_run_size__ = __after_data_run_end__ - __after_data_run_start__;
  __after_data_run_load_end__ = __after_data_run_end__;

  . = ASSERT(__after_data_run_start__ == __after_data_run_end__ || (__after_data_run_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .after_data_run is too large to fit in RAM memory segment");

  __heap_load_start__ = ALIGN(__after_data_run_end__ , 8);
  .heap ALIGN(__after_data_run_end__ , 8) (NOLOAD) : AT(ALIGN(__after_data_run_end__ , 8))
  {
    __heap_start__ = .;
    *(.heap .heap.*)
    . = ALIGN(MAX(__heap_start__ + __HEAPSIZE__ , .), 8);
  }
  __heap_end__ = __heap_start__ + SIZEOF(.heap);
  __heap_size__ = SIZEOF(.heap);
  __heap_load_end__ = __heap_end__;

  . = ASSERT(__heap_start__ == __heap_end__ || (__heap_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .heap is too large to fit in RAM memory segment");

  . = ASSERT(__HEAPSIZE__  == ALIGN(__HEAPSIZE__, 8), "error: .heap section size does not match alignment");

  . = ASSERT(__heap_size__ <= __HEAPSIZE__ , "error: .heap section is larger than specified size");

  __stack_load_start__ = ALIGN(__heap_end__ , 8);
  .stack ALIGN(__heap_end__ , 8) (NOLOAD) : AT(ALIGN(__heap_end__ , 8))
  {
    __stack_start__ = .;
    *(.stack .stack.*)
    . = ALIGN(MAX(__stack_start__ + __STACKSIZE__ , .), 8);
  }
  __stack_end__ = __stack_start__ + SIZEOF(.stack);
  __stack_size__ = SIZEOF(.stack);
  __stack_load_end__ = __stack_end__;

  . = ASSERT(__stack_start__ == __stack_end__ || (__stack_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .stack is too large to fit in RAM memory segment");

  . = ASSERT(__STACKSIZE__  == ALIGN(__STACKSIZE__, 8), "error: .stack section size does not match alignment");

  . = ASSERT(__stack_size__ <= __STACKSIZE__ , "error: .stack section is larger than specified size");

  __stack_process_load_start__ = ALIGN(__stack_end__ , 8);
  .stack_process ALIGN(__stack_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_end__ , 8))
  {
    __stack_process_start__ = .;
    *(.stack_process .stack_process.*)
    . = ALIGN(MAX(__stack_process_start__ + __STACKSIZE_PROCESS__ , .), 8);
  }
  __stack_process_end__ = __stack_process_start__ + SIZEOF(.stack_process);
  __stack_process_size__ = SIZEOF(.stack_process);
  __stack_process_load_end__ = __stack_process_end__;

  __RAM_segment_used_end__ = ALIGN(__stack_end__ , 8) + SIZEOF(.stack_process);
  __RAM_segment_used_size__ = __RAM_segment_used_end__ - __RAM_segment_start__;

  . = ASSERT(__stack_process_start__ == __stack_process_end__ || (__stack_process_end__ - __RAM_segment_start__) <= __RAM_segment_size__ , "error: .stack_process is too large to fit in RAM memory segment");

  . = ASSERT(__STACKSIZE_PROCESS__  == ALIGN(__STACKSIZE_PROCESS__, 8), "error: .stack_process section size does not match alignment");

  . = ASSERT(__stack_process_size__ <= __STACKSIZE_PROCESS__ , "error: .stack_process section is larger than specified size");

  __flashdata_load_start__ = ALIGN(__scratch_y_load_start__ + SIZEOF(.scratch_y) , 4);
  .flashdata ALIGN(__scratch_y_load_start__ + SIZEOF(.scratch_y) , 4) : AT(ALIGN(__scratch_y_load_start__ + SIZEOF(.scratch_y) , 4))
  {
    __flashdata_start__ = .;
    *(.flashdata .flashdata.*)
  }
  __flashdata_end__ = __flashdata_start__ + SIZEOF(.flashdata);
  __flashdata_size__ = SIZEOF(.flashdata);
  __flashdata_load_end__ = __flashdata_end__;

  . = ASSERT(__flashdata_start__ == __flashdata_end__ || (__flashdata_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .flashdata is too large to fit in FLASH memory segment");

  __init_array_load_start__ = ALIGN(__flashdata_end__ , 4);
  .init_array ALIGN(__flashdata_end__ , 4) : AT(ALIGN(__flashdata_end__ , 4))
  {
    __init_array_start__ = .;
    *(.init_array .init_array.*)
  }
  __init_array_end__ = __init_array_start__ + SIZEOF(.init_array);
  __init_array_size__ = SIZEOF(.init_array);
  __init_array_load_end__ = __init_array_end__;

  . = ASSERT(__init_array_start__ == __init_array_end__ || (__init_array_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .init_array is too large to fit in FLASH memory segment");

  __preinit_array_load_start__ = ALIGN(__init_array_end__ , 4);
  .preinit_array ALIGN(__init_array_end__ , 4) : AT(ALIGN(__init_array_end__ , 4))
  {
    __preinit_array_start__ = .;
    KEEP (*(SORT(.preinit_array.*)))
  }
  __preinit_array_end__ = __preinit_array_start__ + SIZEOF(.preinit_array);
  __preinit_array_size__ = SIZEOF(.preinit_array);
  __preinit_array_load_end__ = __preinit_array_end__;

  __FLASH_segment_used_end__ = ALIGN(__init_array_end__ , 4) + SIZEOF(.preinit_array);
  __FLASH_segment_used_size__ = __FLASH_segment_used_end__ - __FLASH_segment_start__;

  . = ASSERT(__preinit_array_start__ == __preinit_array_end__ || (__preinit_array_end__ - __FLASH_segment_start__) <= __FLASH_segment_size__ , "error: .preinit_array is too large to fit in FLASH memory segment");

}

