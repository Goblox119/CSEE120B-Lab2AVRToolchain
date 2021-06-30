# Array of tests to run (in order)
# Each test contains
#   description - 
#   steps - A list of steps to perform, each step can have
#       inputs - A list of tuples for the inputs to apply at that step
#       *time - The time (in ms) to wait before continuing to the next step 
#           and before checking expected values for this step. The time should be a multiple of
#           the period of the system
#       *iterations - The number of clock ticks to wait (periods)
#       expected - The expected value at the end of this step (after the "time" has elapsed.) 
#           If this value is incorrect the test will fail early before completing.
#       * only one of these should be used
#   expected - The expected output (as a list of tuples) at the end of this test
# An example set of tests is shown below. It is important to note that these tests are not "unit tests" in 
# that they are not ran in isolation but in the order shown and the state of the device is not reset or 
# altered in between executions (unless preconditions are used).

tests = [ {'description': 'PINA: 0x3C, PINB: 0x30, PINC: 0x30 => PORTC: 0x29',
    'steps': [ {'inputs': [('PINA',0x3C),('PINB',0x30),('PINC',0x30)], 'iterations': 5 } ],
    'expected': [('PORTD',0x29)],
    },
    {'description': 'PINA: 0x3C, PINB: 0x3C, PINC: Ox3D => PORTD: 0x31',
    'steps': [ {'inputs': [('PINA',0x3C),('PINB',0x3C),('PINC',0x3D)], 'iterations': 5 } ],
    'expected': [('PORTD',0x31)],
    },
    {'description': 'PINA: 0x04, PINB: 0x08, PINC: 0x55 => PORTD: 0x1A',
    'steps': [ {'inputs': [('PINA',0x04),('PINB',0x08),('PINC',0x55)], 'iterations': 5 } ],
    'expected': [('PORTD',0x1A)],
    },
    {'description': 'PINA: 0x10, PINB: 0x20, PINC: 0x1C => PORTD: 0x14',
    'steps': [ {'inputs': [('PINA',0x10),('PINB',0x20),('PINC',0x1C)], 'iterations': 5 } ],
    'expected': [('PORTD',0x14)],
    },
    {'description': 'PINA: 0x10, PINB: 0x57, PINC: 0x57 => PORTD: 0x31',
    'steps': [ {'inputs': [('PINA',0x10),('PINB',0x57),('PINC',0x57)], 'iterations': 5 } ],
    'expected': [('PORTD',0x31)]
    },
    ]
#watch = ['PORTB']

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint

