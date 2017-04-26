import argparse
import sys

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--x',type=float,default=1.0,help=" First argument value")
    parser.add_argument('--y',type=float,default=1.0,help=" Second argument value")
    parser.add_argument('--operations',type=str,default='sub',help=" What is the operation")
    args=parser.parse_args()
    sys.stdout.write(str(calc(args)))

def calc(args):
    if args.operations=='add':
        return args.x + args.y
    elif args.operations=='sub':
        return args.x - args.y
    elif args.operations=='mul':
        return args.x*args.y
    elif args.operations=='div':
        return args.x/args.y

if __name__ == '__main__':
    main()


