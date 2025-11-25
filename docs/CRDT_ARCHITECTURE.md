# CRDT Architecture for Lienzo

## Overview

Lienzo uses **Conflict-free Replicated Data Types (CRDTs)** to enable real-time collaborative editing. Multiple users can edit the same document simultaneously without conflicts, and all changes will eventually converge to the same state.

## CRDT Implementation

### Core Components

#### 1. **CRDTId** (`src/collaboration/crdt.h`)
Unique identifier for each node:
- Format: `siteId:logicalClock` (e.g., `"user1:42"`)
- `siteId`: Unique identifier for each client/user
- `logicalClock`: Monotonically increasing counter per site

#### 2. **CRDTProperty<T>** (`src/collaboration/crdt.h`)
Last-Write-Wins (LWW) property:
- Stores value with timestamp (CRDTId)
- Merge keeps the value with the later timestamp
- Used for properties like position, color, size, etc.

#### 3. **CRDTNode** (`src/collaboration/crdt.h`)
Base class for all design elements:
- **Unique ID**: `CRDTId` for identification
- **Type**: String identifier (e.g., "frame", "shape", "rectangle")
- **Properties**: Key-value map with LWW semantics
- **Children**: Ordered list with tombstones for deletions
- **Tombstones**: Deleted nodes are marked, not removed

#### 4. **CRDTDocument** (`src/collaboration/crdt.h`)
Main document container:
- Manages all CRDT nodes
- Generates unique IDs
- Provides merge operations
- Handles serialization/deserialization

### Vector Data Integration

#### **CRDTVectorShape** (`src/core/vector_crdt.h`)
Wraps `VectorShape` with CRDT properties:
- Syncs shape properties (position, rotation, scale) with CRDT nodes
- All transformations generate CRDT operations
- Properties use LWW for conflict resolution

#### **CRDTFrame** (`src/core/vector_crdt.h`)
CRDT-aware frame container:
- Position and size stored as CRDT properties
- Children (shapes) managed via CRDT child list
- Supports concurrent edits from multiple users

#### **VectorCRDTManager** (`src/core/vector_crdt.h`)
High-level manager:
- Bridges vector shapes and CRDT document
- Provides convenient API for creating/editing frames and shapes
- Handles merge operations
- Rebuilds local state after merges

## How It Works

### Creating Elements

```cpp
VectorCRDTManager manager("user1");

// Create a frame
CRDTId frameId = manager.createFrame(100, 100, 800, 600);

// Create a shape in the frame
auto shape = std::make_shared<Rectangle>(...);
CRDTId shapeId = manager.createShape(frameId, shape);
```

### Editing Elements

```cpp
// Transform a shape (generates CRDT operation)
manager.transformShape(shapeId, dx, dy, scale, rotation);

// Update frame properties
auto frame = manager.getFrame(frameId);
frame->setPosition(newX, newY, manager.getDocument());
```

### Merging Changes

```cpp
// Receive document state from another user
VectorCRDTManager otherManager("user2");
// ... otherManager has been modified ...

// Merge their changes into your document
manager.merge(otherManager);
// Local state is automatically rebuilt
```

## Conflict Resolution

### Last-Write-Wins (LWW) for Properties
- When two users edit the same property (e.g., position)
- The operation with the later timestamp wins
- Timestamps are compared by logical clock, then site ID

### Tombstones for Deletions
- Deleted nodes are marked with a deletion timestamp
- Not removed from the structure
- Can be "resurrected" if a later operation adds them back

### Ordered Lists for Children
- Children are stored with add/remove timestamps
- Deleted children are filtered out when reading
- Concurrent inserts are handled by timestamp ordering

## Network Synchronization

### Serialization
```cpp
std::string state = manager.getDocument().serialize();
// Send over network
```

### Deserialization
```cpp
VectorCRDTManager remoteManager("remote");
remoteManager.getDocument().deserialize(receivedState);
manager.merge(remoteManager);
```

## Benefits

1. **No Conflicts**: Operations are commutative and idempotent
2. **Eventual Consistency**: All replicas converge to the same state
3. **Offline Support**: Changes can be made offline and merged later
4. **No Central Authority**: No need for a central server to resolve conflicts
5. **Deterministic**: Same operations always produce the same result

## Performance Considerations

- **Memory**: Tombstones are kept, so deleted nodes consume memory
  - Can be garbage collected after a time period
- **Merge Complexity**: O(n) where n is number of nodes
- **Serialization**: Needs efficient binary or JSON format
- **Network**: Only need to send changes, not full state

## Future Enhancements

1. **Garbage Collection**: Remove old tombstones after merge window
2. **Compression**: Efficient serialization format
3. **Incremental Sync**: Only send changes, not full state
4. **Vector Clocks**: More sophisticated ordering for complex operations
5. **Operation Logs**: Keep history for undo/redo

