import wcics.server.routes

from wcics.utils.time import get_time
from wcics.database.models import Organizations
from wcics.database.utils import db_commit

## Perform any init that is assumed to exist

# Create main organization
Organizations.add(oid = 'main', name = 'Main Organization', desc = 'main organization thing')

# This organization should eventually be created through our interface (but said interface does not yet exist...)
Organizations.add(oid = "test", name = "Test organization", desc = "I am a test")

db_commit()

print("\nOrganizations were sucessfully created!\n")