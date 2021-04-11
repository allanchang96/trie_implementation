#include <iostream>
#include <string>
#include <vector>

struct Trie 
{
  std::string nodeVal = "";
  std::string data = "";
  int numOfBranches = 0;
  bool isRoot = false;
  Trie *arr[26];
};

void insert(Trie *&node, std::string target, int iTarget)
{
  // If empty node and no branches
  if(node->nodeVal == "" && node->numOfBranches == 0){
    node->nodeVal = target.substr(iTarget);
    node->data = target;
  }
  else{
    int iNode = 0;
    while(iTarget < target.size() && iNode < node->nodeVal.size() && target[iTarget] == node->nodeVal[iNode]){
      iTarget++;
      iNode++;
    }
    if(iTarget == target.size()){
      // Matches the node
      node->data = target;
    }
    else if(iNode == node->nodeVal.size()){
      // Reached end of nodeVal
      if(node->arr[target[iTarget] - 'a']){
        // Next searching node exists
        insert(node->arr[target[iTarget] - 'a'], target, iTarget);
      }
      else{
        // Next searching node does not exist
        Trie *newBranch = new Trie();
        node->arr[target[iTarget] - 'a'] = newBranch;
        node->numOfBranches++;
        insert(node->arr[target[iTarget] - 'a'], target, iTarget);
      }
    }
    else{
      // Does not match
      // Create new branch
      Trie *prefixBranch = new Trie();
      // Take the prefix of the current node and assign it to prefixBranch.
      prefixBranch->nodeVal = node->nodeVal.substr(0, iNode);
      // Remove the prefix from the current node.
      node->nodeVal = node->nodeVal.substr(iNode);
      // Create copy of current node
      Trie *tmp = node;
      // Attach prefix onto where current node was originally
      node = prefixBranch;
      // Attach current node onto prefix branch
      prefixBranch->arr[tmp->nodeVal[0] - 'a'] = tmp;
      prefixBranch->numOfBranches++;
      // Create a new Trie called newBranch and attach it to prefixBranch.
      Trie *newBranch = new Trie();
      prefixBranch->arr[target[iTarget] - 'a'] = newBranch;
      prefixBranch->numOfBranches++;
      insert(prefixBranch->arr[target[iTarget] - 'a'], target, iTarget);
    }
  }
}

bool remove(Trie *&node, std::string target, int iTarget)
{
  int iNode = 0;
  while(iTarget < target.size() && iNode < node->nodeVal.size() && target[iTarget] == node->nodeVal[iNode]){
    iTarget++;
    iNode++;
  }
  if(iTarget == target.size()){
    // Matches the node
    if(target == node->data){
      // Data exists in the trie
      // Delete data entry
      node->data = "";
      if(node->numOfBranches == 0){
        // Leaf node
        node->nodeVal = "";
        // Free node memory if not root and tell layer above to reduce numOfBranches
        if(!node->isRoot){
          delete node;
          node = nullptr;
          return true;
        }
        else{
          return false;
        }
      }
      else if(node->numOfBranches == 1){
        // Collapse the branch
        for(const auto t : node->arr){
          if(t){
            // Found remaining branch
            t->nodeVal = node->nodeVal + t->nodeVal;
            // Copy the isRoot value so root is maintained
            t->isRoot = node->isRoot;
            // Copy memory address to free
            Trie *tmp = node;
            node = t;
            delete tmp;
            tmp = nullptr;
            return false;
          }
        }
      }
    }
    else{
      // Data does not exist
      return false;
    }
  }
  else if(iNode == node->nodeVal.size()){
    // Reached end of nodeVal
    if(node->arr[target[iTarget] - 'a']){
      // Next searching node exists
      if(remove(node->arr[target[iTarget] - 'a'], target, iTarget)){
        node->numOfBranches--;
        if(node->data == "" && node->numOfBranches == 1){
          // Collapse the branch
          for(const auto t : node->arr){
            if(t){
              // Found remaining branch
              t->nodeVal = node->nodeVal + t->nodeVal;
              // Copy the isRoot value so root is maintained
              t->isRoot = node->isRoot;
              // Copy memory address to free
              Trie *tmp = node;
              node = t;
              delete tmp;
              tmp = nullptr;
              return false;
            }
          }
        }
      }
    }
    else{
      // Data does not exist
      return false;
    }
  }
  // Does not exist
  return false;
}

void printTrie(Trie *node, int layer, std::string prefix)
{
  std::cout << "Layer: " << layer 
  << " Prefix: " << prefix
  << " Node: " << node->nodeVal
  << " Data: " << node->data
  << std::endl;
  for(auto t : node->arr){
    if(t){
      printTrie(t, layer + 1, prefix + node->nodeVal);
    }
  }
}

std::vector<std::string> searchTrie(Trie *node, std::string target, int iTarget)
{
  std::vector<std::string> res;
  int iNode = 0;
  while(iTarget < target.size() && iNode < node->nodeVal.size() && target[iTarget] == node->nodeVal[iNode]){
    iTarget++;
    iNode++;
  }
  if(iTarget == target.size()){
    // Target matched, print the Trie from this node
    if(!node->data.empty()){
      res.push_back(node->data);
    }
    for(const auto t : node->arr){
      if(t){
        std::vector<std::string> branchElements = searchTrie(t, target, iTarget);
        res.insert(res.end(), branchElements.begin(), branchElements.end());
      }
    }
  }
  else if(iNode == node->nodeVal.size()){
    std::vector<std::string> branchElements = searchTrie(node->arr[target[iTarget] - 'a'], target, iTarget);
    res.insert(res.end(), branchElements.begin(), branchElements.end());
  }
  return res;
}

bool exists(Trie *node, std::string target, int iTarget)
{
  int iNode = 0;
  while(iTarget < target.size() && iNode < node->nodeVal.size() && target[iTarget] == node->nodeVal[iNode]){
    iTarget++;
    iNode++;
  }
  if(iTarget == target.size()){
    // Target matched
    if(!node->data.empty()){
      return true;
    }
    else{
      return false;
    }
  }
  else if(iNode == node->nodeVal.size()){
    if(node->arr[target[iTarget] - 'a']){
      return exists(node->arr[target[iTarget] - 'a'], target, iTarget);
    }
    else{
      return false;
    }
  }
  else{
    return false;
  }
}

int main(void){
  Trie *root = new Trie();
  root->isRoot = true;
  insert(root, "a", 0);
  insert(root, "ab", 0);
  insert(root, "abc", 0);
  insert(root, "abcd", 0);
  insert(root, "abdeft", 0);
  std::cout << exists(root, "abcdeft", 0) << std::endl;
  std::cout << exists(root, "abdeft", 0) << std::endl;
  std::vector<std::string> result = searchTrie(root, "", 0);
  for(const auto s : result){
    std::cout << s << std::endl;
  }
  printTrie(root, 0, "");
}