# Graphs
graphs_mandatory = [
    'BxB1100_999_54_2017',
    'CBQsc100_200_11_1517',
    'CBQsv1000_77_150.txt',
    'CBQsv10_22_61.txt',
    'DG100.txt',
    'GRD22154753987',
    'GRD505041100',
    'GRD99704280',
    'Gf12345_12111222_457_15',
    'KC',
    'KD',
    'KM',
    'Octo.txt',
    'PG64.txt',
    'R1999999_10123123_1',
    'bxb15_22_10',
    'bxb777_999_12',
    'q10.corr',
    'q13.corr',
    'school1.col'
  ]

graphs_optional = [
    'A1H75_18',
    'A7H4H_18',
    'A_1515153016_18',
    'C1599_1777',
    'Cne140_4',
    'R1789_875431_10',
    'R22_99_15',
    'lux1',
    'lux2',
    'lux3',
    'lux4'
]

# Put all the source code here except for tests
sources = [ 'algo.c', 'array.c', 'graph.c', 'log.c', 'util.c', 'queue.c', 'order.c' ]
cangrejo_estelar_lib = shared_library('cangrejo estelar', sources)


### Unit tests ###
test_array = executable('test_array', 'array_test.c', link_with: cangrejo_estelar_lib)
test_graph = executable('test_graph', 'graph_test.c', link_with: cangrejo_estelar_lib)
test_util = executable('test_util', 'util_test.c', link_with: cangrejo_estelar_lib)
test_parse = executable('test_parse', 'parse_test.c', link_with: cangrejo_estelar_lib)
test_queue = executable('test_queue', 'queue_test.c', link_with: cangrejo_estelar_lib)
test_order = executable('test_order', 'order_test.c', link_with: cangrejo_estelar_lib)
test_algo = executable('test_algo', 'algo_test.c', link_with: cangrejo_estelar_lib)

test('array', test_array, suite: 'unit test')
test('graph', test_graph, suite: 'unit test')
test('util', test_util, suite: 'unit test')
test('queue', test_queue, suite: 'unit test')
test('order', test_order, suite: 'unit test')
test('algo', test_algo, suite: 'unit test')

# Generate tests for each graph
foreach gm: graphs_mandatory
  fullpath = join_paths('../graphs', gm)
  test('graph ' + gm, test_parse, suite: 'unit test', args: fullpath, timeout: 300)
endforeach

foreach gm: graphs_optional
  fullpath = join_paths('../graphs', gm)
  test('graph ' + gm, test_parse, suite: 'unit test optional', args: fullpath, timeout: 300)
endforeach

### Integration tests ###
test_full = executable('test_full', 'full_test.c', link_with: cangrejo_estelar_lib)

foreach gm: graphs_mandatory
  fullpath = join_paths('../graphs', gm)
  test('graph ' + gm, test_full, suite: 'integration test', args: fullpath, timeout: 300)
endforeach

foreach gm: graphs_optional
  fullpath = join_paths('../graphs', gm)
  test('graph ' + gm, test_full, suite: 'integration test optional', args: fullpath, timeout: 300)
endforeach
