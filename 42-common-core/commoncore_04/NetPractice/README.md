*This project has been created as part of the 42 curriculum by eulee.*

## Description

NetPractice is a networking exercise project from the 42 curriculum.
The goal is to configure small-scale networks by correctly assigning
IP addresses, subnet masks, and routes so that all devices can communicate.

## Instructions

### Running the training interface
```bash
./run.sh
```

If `run.sh` does not work, run it manually:
```bash
python3 -m http.server 49242
```

Then open your browser and go to `http://localhost:49242`.

Enter your intranet login in the field provided, then select a level.

### Exporting configurations

Once a level is solved, click **Get my config** to download the configuration file.
Do this for every level before moving to the next one.

### Submission

10 exported configuration files (one per level) must be placed at the root of the repository,
along with this README.md.

## Resources

### References
- [TCP/IP Guide](http://www.tcpipguide.com/)
- [Subnet Calculator](https://www.subnet-calculator.com/)

### Networking concepts studied
- TCP/IP addressing
- Subnet masks and CIDR notation
- Network and broadcast addresses
- Default gateways
- Routers and switches
- Routing tables
- OSI layers

### AI usage
- **Claude (Anthropic)**: Used to understand subnetting concepts,
  practice IP range calculations, write README.md and prepare for peer evaluation.
