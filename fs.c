#define LBA_OFFSET_SIZEOF 8
#define FILESYSTEM_HEADER_SIZE 100000

void *filesystem_read_from_header_index(size_t record_index, size_t size) {
  size_t offset = read(record_index * LBA_OFFSET_SIZEOF);
  return read(offset + FILESYSTEM_HEADER_SIZE * LBA_OFFSET_SIZEOF);
}

typedef struct {
  size_t index;
} RecordPtr;

typedef struct {
  RecordPtr parent;
  char name[255];
  size_t data_size;
  void *data;

  char _padding[1024];
} Record;

typedef struct {
  void *data;
  size_t size;
  size_t index;

  void *(*next)(void);
} DataReader;

DataReader make_data_reader(void *data, size_t size) {
  DataReader reader;
  reader.data = data;
  reader.size = size;
  reader.index = 0;
  reader.next = &_data_reader_next;

  return reader;
}

void *_data_reader_next(DataReader *self, size_t size) {
  assert(self->index + size < self->size && "Bounds check failed!");

  void *mem =self->data + self->index;
  self->index += size;
  return mem;
}

Record *load_record_from_raw_data(void *raw_data) {
  DataReader reader = make_data_reader(raw_data, sizeof Record);

  Record *record = malloc(sizeof Record);
  record->parent = (RecordPtr)reader.next(&reader, sizeof record->parent);
  strcpy(record->name, (char*)reader.next(&reader, sizeof record->name));
  record->data_size = (size_t)reader.next(&reader, sizeof record->data_size);
  // TODO: paging?
  record->data = malloc(record->data_size);
  memcpy(record->data, reader.next(&reader, record->data_size));
  
  // lul
  memcpy(record->padding, reader.next(&reader, sizeof record->padding));

  return record;
}

void free_record(Record *record) {
  if (record->data) free(record->data);
  free(record);
}